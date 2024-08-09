#include <Systems/CommandSystem.h>


#include <Entity.h>
#include <Raylib.h>
#include <Utils.h>


pd::CommandSystem::CommandSystem(observer_ptr<EntityManager> entityManager)
	: System(entityManager)
{
	m_componentGroups = {
		{utils::stringHash("CommandTuple"), {typeid(ActorCommand)}},
		{utils::stringHash("ActorMovingTuple"), {
			typeid(ActorIsMoving),
			typeid(ParticleSystemComponent),
		}},
		{utils::stringHash("ActorShootingTuple"), {
			typeid(ActorIsShooting),
			typeid(ParticleSystemComponent),
		}},
	};
}

void pd::CommandSystem::update(f32 timeElapsedS)
{
	auto* input = m_pEntityManager->getSingletonComponent<Input>();
	auto* camera = m_pEntityManager->getSingletonComponent<Camera>();

	//auto& playerCommand = *getPlayerCommand().command;

	//if (playerCommand.m_isLocalPlayer)
	//{
	//	Vector2 move = {
	//		input->keyDown(KEY_A) ? -1.0f : input->keyDown(KEY_D) ? 1.0f : 0.0f,
	//		input->keyDown(KEY_W) ? 1.0f : input->keyDown(KEY_S) ? -1.0f : 0.0f,
	//	};

	//	playerCommand.m_moveXY = Vector2Normalize(move);
	//	playerCommand.m_shoot = input->mouseButtonPressed(MOUSE_BUTTON_LEFT);
	//}

	Vector2 move = {
		input->keyDown(KEY_A) ? -1.0f : input->keyDown(KEY_D) ? 1.0f : 0.0f,
		input->keyDown(KEY_W) ? 1.0f : input->keyDown(KEY_S) ? -1.0f : 0.0f,
	};
	bool shoot = input->mouseButtonPressed(MOUSE_BUTTON_LEFT);

	for (const auto& t : getCommandTuples())
	{
		if (!t.command->m_isPlayer)
			continue;

		t.command->m_moveXY = Vector2Normalize(move);
		t.command->m_shoot = shoot;

		Vector2 mousePosition = input->mousePosition();
		// TODO: Shared behavior? ScreentToWolrd(Camera camera, Input input)
		t.command->m_aimAt = camera->screenToWorld(mousePosition.x, mousePosition.y);
	}

	for (const auto& t : getActorIsMovingTuples())
	{
		t.movingState->m_isMoving = Vector2Length(move) != 0.0f;
		t.particleSystem->m_isPlaying = t.movingState->m_isMoving;
	}

	for (const auto& t : getActorIsShootingTuples())
	{
		t.shootingState->m_isShooting = shoot;
		t.particleSystem->m_isPlaying = t.shootingState->m_isShooting;
	}
}

//Vector2D ConvertScreenToWorld(const Vector2D& screenCoords, const Camera& camera) {
//	// Adjust for camera position and zoom
//	Vector2D worldCoords;
//	worldCoords.x = (screenCoords.x - screenWidth / 2) / camera.zoom + camera.position.x;
//	worldCoords.y = -(screenCoords.y - screenHeight / 2) / camera.zoom + camera.position.y;
//	return worldCoords;
//}

//auto pd::CommandSystem::getPlayerCommand() -> PlayerCommandTuple&
//{
//	m_pEntityManager->getPlayerCommand(m_playerCommand);
//	ASSERT_MSG(m_playerCommand.command != nullptr, "No entity with PlayerCommand component found. Are you sure you created one?");
//	return m_playerCommand;
//}

auto pd::CommandSystem::getCommandTuples() -> std::vector<CommandTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("CommandTuple"))))
		m_pEntityManager->getCommandTuples(m_commandTuples);
	return m_commandTuples;
}

auto pd::CommandSystem::getActorIsMovingTuples() -> std::vector<ActorIsMovingTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("ActorMovingTuple"))))
		m_pEntityManager->getActorMovingTuples(m_actorMovingTuples);
	return m_actorMovingTuples;
}

auto pd::CommandSystem::getActorIsShootingTuples() -> std::vector<ActorIsShootingTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("ActorShootingTuple"))))
		m_pEntityManager->getActorShootingTuples(m_actorShootingTuples);
	return m_actorShootingTuples;
}
