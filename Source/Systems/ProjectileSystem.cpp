#include <Systems/ProjectileSystem.h>


#include <Entity.h>
#include <Utils.h>


pd::ProjectileSystem::ProjectileSystem(observer_ptr<EntityManager> entityManager) : System(entityManager)
{
	m_componentGroups = {
		{utils::stringHash("ProjectileSpawnerTuple"), {
			typeid(ActorCommand),
			typeid(ProjectileSpawner),
			typeid(Transform),
		}},
		{utils::stringHash("ProjectileSimulationTuple"), {
			typeid(Transform),
			typeid(Projectile),
		}},
	};
}

void pd::ProjectileSystem::update(f32 timeElapsedS)
{
	for (const auto& t : getProjectileSpawnerTuples())
	{
		const auto& transform = t.transform;
		const auto& spawner = t.spawner;
		const auto& command = t.command;

		if (command->m_shoot)
		{
			auto projectile = m_pEntityManager->createEntity("Projectile-" + Guid::Generate().toString());
			TransformArgs tArgs;
			tArgs.position = transform->position();
			tArgs.rotation = transform->rotation();
			projectile->addComponent<Transform>(&tArgs);
			ProjectileArgs pArgs;
			pArgs.lifetimeSeconds = MAX_PROJECTILE_LIFETIME;
			const f32 rotation = (transform->rotation() - 90.0f) * (PI / 180.0f);
			const auto& direction = Vector2Normalize({ cos(rotation), sin(rotation) });
			pArgs.velocity = Vector2Scale(direction, spawner->m_projectileSpeed);
			pArgs.damage = spawner->m_projectileDamage;
			pArgs.hitRadius = spawner->m_projectileHitRadius;
			projectile->addComponent<Projectile>(&pArgs);
			GraphicsArgs gArgs;
			gArgs.textureName = "light_shell";
			auto& assets = ServiceGetter::get<AssetManager>();
			auto* texturePtr = assets.getTexture(gArgs.textureName);
			ASSERT(texturePtr);
			gArgs.size = { (f32)texturePtr->width, (f32)texturePtr->height };
			gArgs.pivot = { 0.5f, 0.5f };
			gArgs.drawLayer = 0;  // TODO: named layers / Layer service
			projectile->addComponent<Graphics>(&gArgs);
		}
	}

	for (const auto& t : getProjectileSimulationTuples())
	{
		const auto transform = t.transform;
		const auto projectile = t.projectile;
	
		transform->translate(Vector2Scale(projectile->m_velocity, timeElapsedS));
		projectile->m_lifetimeSeconds -= timeElapsedS;

		if (projectile->m_lifetimeSeconds <= 0.0f)
		{
			// TODO: Kill entity
			m_pEntityManager->destroyEntity(projectile->m_owner);
		}
	}
}

auto pd::ProjectileSystem::getProjectileSpawnerTuples() -> std::vector<ProjectileSpawnerTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("ProjectileSpawnerTuple"))))
		m_pEntityManager->getProjectileSpawnerTuples(m_projectileSpawnerTuples);
	return m_projectileSpawnerTuples;
}

auto pd::ProjectileSystem::getProjectileSimulationTuples() -> std::vector<ProjectileSimulationTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("ProjectileSimulationTuple"))))
		m_pEntityManager->getProjectileSimulationTuples(m_projectileSimulationTuples);
	return m_projectileSimulationTuples;
}
