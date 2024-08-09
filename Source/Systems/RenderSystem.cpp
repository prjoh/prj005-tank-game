#include <Systems/RenderSystem.h>


#if (DEBUG_BUILD)
#include <chrono>
#endif
#include <algorithm>

#include <Components/Camera.h>
#include <Components/Graphics.h>
#include <Game.h>
#include <Entity.h>
#include <Utils.h>
#include <Systems/PhysicsSystem.h>


using namespace pd;


pd::RenderSystem::RenderSystem(observer_ptr<EntityManager> entityManager) : System(entityManager)
{
	m_componentGroups = {
		{utils::stringHash("GraphicsTuple"), {
			typeid(Transform),
			typeid(Graphics),
		}},
		{utils::stringHash("ParticleSystemTuple"), {
			typeid(Transform),
			typeid(ParticleSystemComponent),
		}},
		{utils::stringHash("DynPhysicsDebugTuple"), {
			typeid(DynamicPhysicsComponent),
			typeid(Transform),
		}},
		{utils::stringHash("StaPhysicsDebugTuple"), {
			typeid(StaticPhysicsComponent),
			typeid(Transform),
		}},
		{utils::stringHash("KinPhysicsDebugTuple"), {
			typeid(KinematicPhysicsTuple),
			typeid(Transform),
		}},
	};
}

// DELETE
// TODO: I think it is time to rethink the render system.
//   How can we structure all render data such that we draw in groups of drawLayers?
//   Each frame, we have to iterate all Graphics/Tilemap components and then put them in a datastructure!!!
//   It is probably a good idea to make a singleton for RenderSystem, 
//   which caches last result and only gets updated if some data is set as dirty
// UPDATE: How about we make a mille feuille of graphics/particles

// TODO: Implement data structure to render only necessary tiles
void RenderSystem::update(f32 timeElapsedS)
{
	auto* camera = m_pEntityManager->getSingletonComponent<Camera>();
	ASSERT_MSG(camera, "No camera defined!");

	const auto& entityRenderTuples = getEntityRenderTuples();
	const auto& particleRenderTuples = getParticlesRenderTuples();

	BeginDrawing();

	ClearBackground(BLACK);

	render(*camera, entityRenderTuples, particleRenderTuples);
	debugRender(*camera);

	EndDrawing();
}

void pd::RenderSystem::render(
	const Camera& camera, 
	const std::vector<EntityRenderTuple>& entityRenderTuples,
	const std::vector<ParticlesRenderTuple>& particleRenderTuples) const
{

	for (const auto& t : entityRenderTuples)
	{
		//auto p2 = std::chrono::high_resolution_clock::now();

		auto& graphics = *t.graphics;
		const auto& transform = *t.transform;

		// TODO: Camera matrix is not applied correctly here
		const auto& graphicPosition = Vector2Subtract(transform.position(), camera.m_position);
		const auto& graphicRotation = transform.rotation();
		const auto& graphicScale = transform.scale();

		const auto& texture = *graphics.m_pTexture;

		// TODO: Remove the getter and modify m_sourceRect directly
		auto srcRect = graphics.srcRect();
		const auto& destRect = graphics.dstRect(graphicPosition, graphicScale);
		const auto& origin = graphics.origin(graphicScale);

		if (graphics.m_animated)
			srcRect.x += srcRect.width * graphics.m_currentFrame;

		//auto p3 = std::chrono::high_resolution_clock::now();

		DrawTexturePro(
			texture,
			srcRect,
			destRect,
			origin,
			graphicRotation + graphics.m_rotation,
			WHITE
		);

		//DrawCircle((s32)graphicPosition.x, (s32)graphicPosition.y, 5, RED);
		// TODO: MEASURE END

		//drawCalls++;
		//auto p4 = std::chrono::high_resolution_clock::now();
		//calcTime += std::chrono::duration_cast<std::chrono::microseconds>(p3 - p2).count();
		//renderTime += std::chrono::duration_cast<std::chrono::microseconds>(p4 - p3).count();

		//if (graphics.m_animated)
		//{
		//	const std::string frameText = "Frame: " + std::to_string(graphics.m_currentFrame);
		//	DrawText(frameText.c_str(), 20, 40, 20, WHITE);
		//}
	}

#if (DEBUG_BUILD)
	u64 liveParticles = 0;
#endif

	for (const auto& t : particleRenderTuples)
	{
		const auto& ps = t.particleSystem;
		const auto& transform = t.transform;

		//transform->rotate(timeElapsedS * 4.0f);  // TODO: Remove; for testing

		const Texture2D texture = *ps->m_pTexture;
		const Vector2 size = { (f32)texture.width, (f32)texture.height };
		const Rectangle src{ 0.0f, 0.0f, size.x, size.y };
		Rectangle dst{ 0.0f, 0.0f, size.x, size.y };
		const Vector2 origin = { size.x * 0.5f, size.y * 0.5f };
		for (const auto& p : ps->getLiveParticles())
		{
			auto position = p->m_position;
			if (!ps->m_worldSpaceEmission)
			{
				position = Vector2Add(position, transform->position());

				// TODO: Position is not affected by rotation

			}
			position = Vector2Subtract(position, camera.m_position);
			dst.x = position.x;
			dst.y = position.y;
			DrawTexturePro(
				texture,
				src,
				dst,
				origin,
				p->m_rotation,
				p->color()
			);
		}

#if (DEBUG_BUILD)
		liveParticles += (u32)ps->getLiveParticles().size();
#endif
	}

#if (DEBUG_BUILD)
	drawStats(liveParticles);
#endif
}

#if (DEBUG_BUILD)
void pd::RenderSystem::debugRender(const Camera& camera)
{
	const auto& dynamicPhysicsDebugTuples = getDynPhysicsDebugRenderTuples();
	const auto& staticPhysicsDebugTuples = getStaPhysicsDebugRenderTuples();
	const auto& kinematicPhysicsDebugTuples = getKinPhysicsDebugRenderTuples();

	const auto renderPhysicsComponent = [](const Camera& camera, const PhysicsComponent* pc, const Transform* t, const Color& color)
	{
		const Vector2 position = Vector2Subtract(t->position(), camera.m_position);
		const f32 rotation = -t->rotation() * DEG2RAD;  // World is CW rotation / Physics Wrold is CCW rotation

		const auto* shape = pc->m_pShape.get();
		const auto& type = shape->GetType();
		switch (type)
		{
		case b2Shape::e_polygon:
		{
			const auto* polyShape = static_cast<const b2PolygonShape*>(shape);
			std::vector<Vector2> verts;
			for (s32 i = 0; i < polyShape->m_count; ++i)
			{
				auto x = polyShape->m_vertices[i].x;
				auto y = polyShape->m_vertices[i].y;

				auto vert = Vector2Multiply(
					{
						x * cosf(rotation) - y * sinf(rotation),
						x * sinf(rotation) + y * cosf(rotation)
					},
					WORLD2SCREEN
				);
				vert = Vector2Add(vert, position);
				verts.push_back(vert);
			}
			verts.push_back(verts[0]);

			DrawLineStrip(verts.data(), verts.size(), color);
			break;
		}
		default:
			break;
		}
	};

	for (const auto& t : staticPhysicsDebugTuples)
		renderPhysicsComponent(camera, t.physicsComponent, t.transform, YELLOW);

	for (const auto& t : dynamicPhysicsDebugTuples)
		renderPhysicsComponent(camera, t.physicsComponent, t.transform, GREEN);


	auto* player = m_pEntityManager->getEntityById("Player");
	auto* t = player->getComponent<Transform>();
	auto* physics = player->getComponent<DynamicPhysicsComponent>();
	// TODO: Camera matrix is not applied correctly here
	const auto& position = Vector2Subtract(t->position(), camera.m_position);
	const auto& rotation = t->rotation();
	const auto& scale = t->scale();

	const auto& physicsRot = physics->m_pBody->GetAngle() * RAD2DEG;

	const std::string positionText = "Pos: " + std::to_string((s32)t->position().x) + "," + std::to_string((s32)t->position().y);
	const std::string rotationText = "Rot(T): " + std::to_string((s32)t->rotation());
	const std::string rotationText2 = "Rot(P): " + std::to_string((s32)physicsRot);

	DrawRectangle(position.x + 100, position.y - 150, 200, 150, { 80, 80, 80, 200 });
	DrawText(positionText.c_str(), position.x + 120, position.y - 130, 20, MAGENTA);
	DrawText(rotationText.c_str(), position.x + 120, position.y - 100, 20, MAGENTA);
	DrawText(rotationText2.c_str(), position.x + 120, position.y - 70, 20, MAGENTA);
}

void pd::RenderSystem::drawStats(u32 liveParticles) const
{
	//const std::string tupleTimeText = "tupleTime: " + std::to_string(tupleTime / 1000) + "." + std::to_string((tupleTime % 1000) / 100) + "ms";
	//const std::string calcTimeText = "calcTime: " + std::to_string(calcTime / 1000) + "." + std::to_string((calcTime % 1000) / 100) + "ms";
	//const std::string renderTimeText = "renderTime: " + std::to_string(renderTime / 1000) + "." + std::to_string((renderTime % 1000) / 100) + "ms";
	//const std::string drawsText = "draws: " + std::to_string(drawCalls);

	//DrawText(tupleTimeText.c_str(), 20, 20, 20, MAGENTA);
	//DrawText(calcTimeText.c_str(), 20, 60, 20, MAGENTA);
	//DrawText(renderTimeText.c_str(), 20, 100, 20, MAGENTA);
	//DrawText(drawsText.c_str(), 20, 140, 20, MAGENTA);

	u32 rows = (u32)Game::s_SystemTimes.size();
	DrawRectangle(Game::s_ScreenWidth - 340, 10, 330, 100 + 40 * (rows + 2), { 80, 80, 80, 200 });

	auto frameEnd = std::chrono::high_resolution_clock::now();
	auto frameTime = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - Game::s_FrameStart).count();

	auto frameTimeMilli = frameTime / 1000;
	auto frameTimeMicro = frameTime % 1000;

	const std::string frameTimeText = "FrameTime: " + std::to_string(frameTimeMilli) + "." + std::to_string(frameTimeMicro / 100) + "ms";
	DrawText(frameTimeText.c_str(), Game::s_ScreenWidth - 300, 20, 20, MAGENTA);

	for (u32 i = 0; i < Game::s_SystemTimes.size(); ++i)
		DrawText(Game::s_SystemTimes[i].c_str(), Game::s_ScreenWidth - 300, 60 + 40 * i, 20, MAGENTA);

	auto systemTime = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - m_systemStart).count();
	auto systemTimeMilli = systemTime / 1000;
	auto systemTimeMicro = systemTime % 1000;
	const std::string renderSystemTimeText = "RenderSystem: " + std::to_string(systemTimeMilli) + "." + std::to_string(systemTimeMicro / 100) + "ms";
	DrawText(renderSystemTimeText.c_str(), Game::s_ScreenWidth - 300, 60 + 40 * rows, 20, MAGENTA);

	const std::string entitiesText = "Live Entites: " + std::to_string(m_pEntityManager->getLiveEntites());
	DrawText(entitiesText.c_str(), Game::s_ScreenWidth - 300, 60 + 40 * (rows + 1), 20, GREEN);

	const std::string particlesText = "Live Particles: " + std::to_string(liveParticles);
	DrawText(particlesText.c_str(), Game::s_ScreenWidth - 300, 60 + 40 * (rows + 2), 20, GREEN);
}
#endif

auto RenderSystem::getEntityRenderTuples() -> std::vector<EntityRenderTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("GraphicsTuple"))))
		m_pEntityManager->getEntityRenderTuples(m_entityRenderTuples);

	// TODO: This sort needs to optimized!!!
	std::sort(m_entityRenderTuples.begin(), m_entityRenderTuples.end(), [](const EntityRenderTuple& a, const EntityRenderTuple& b) {
		return a.graphics->m_drawLayer < b.graphics->m_drawLayer;
	});

	return m_entityRenderTuples;
}

auto pd::RenderSystem::getParticlesRenderTuples() -> std::vector<ParticlesRenderTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("GraphicsTuple"))))
		m_pEntityManager->getParticlesRenderTuples(m_particleRenderTuples);

	// TODO: This sort needs to optimized!!!
	std::sort(m_particleRenderTuples.begin(), m_particleRenderTuples.end(), [](const ParticlesRenderTuple& a, const ParticlesRenderTuple& b) {
		return a.particleSystem->m_drawLayer < b.particleSystem->m_drawLayer;
	});

	return m_particleRenderTuples;
}

#if (DEBUG_BUILD)
auto pd::RenderSystem::getDynPhysicsDebugRenderTuples() -> std::vector<DynamicPhysicsTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("DynPhysicsDebugTuple"))))
		m_pEntityManager->getDynamicPhysicsTuples(m_dynPhysicsTuples);
	return m_dynPhysicsTuples;
}

auto pd::RenderSystem::getStaPhysicsDebugRenderTuples() -> std::vector<StaticPhysicsTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("DynPhysicsDebugTuple"))))
		m_pEntityManager->getStaticPhysicsTuples(m_staPhysicsTuples);
	return m_staPhysicsTuples;
}

auto pd::RenderSystem::getKinPhysicsDebugRenderTuples() -> std::vector<KinematicPhysicsTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("DynPhysicsDebugTuple"))))
		m_pEntityManager->getKinematicPhysicsTuples(m_kinPhysicsTuples);
	return m_kinPhysicsTuples;
}
#endif
