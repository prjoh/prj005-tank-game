#include <Systems/PlayerController.h>


#include <Entity.h>
#include <Utils.h>


pd::PlayerController::PlayerController(observer_ptr<EntityManager> entityManager)
	: System(entityManager)
{
	m_componentGroups = {
		{utils::stringHash("ControllerTuple"), {
			typeid(DynamicPhysicsComponent),
			typeid(Transform),
			typeid(ActorCommand),
			typeid(Graphics),
		}},
	};
}

auto pd::PlayerController::getPlayerControllerTuples() -> std::vector<PlayerControllerTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("ControllerTuple"))))
		m_pEntityManager->getControllerTuples(m_controllerTuples);
	return m_controllerTuples;
}

void pd::PlayerController::initialize()
{
	getPlayerControllerTuples();
}

void pd::PlayerController::fixedUpdate(f32 fixedTimeElapsedS)
{
	const f32 speed = 175.0f;
	const f32 angularSpeed = 350.0f;
	const f32 maxSpeed = 7.5f;
	const f32 maxAngularSpeed = 0.9f;
	const f32 timeStep = 1 / 60.0f;

	for (const auto& t : getPlayerControllerTuples())
	{
		auto& physicsComponent = *t.physicsComponent;
		//auto& transform = *t.transform;
		auto& command = *t.command;
		auto& graphics = *t.graphics;

		// TODO: Should we abstract this to component and call it m_isReadOnly?
		if (command.m_observer)
			continue;

		if (command.m_isPlayer)
		{
			f32 rotation = physicsComponent.m_pBody->GetAngle();
			b2Vec2 direction = { cosf(rotation), sinf(rotation) };
			direction.Normalize();

			b2Vec2 linearVel = physicsComponent.m_pBody->GetLinearVelocity();
			f32 angularVel = physicsComponent.m_pBody->GetAngularVelocity();

			auto& movement = command.moveXY();

			// World is CW rotation / Physics Wrold is CCW rotation
			if (movement.x < 0.0f)
				angularVel = b2Min(angularVel + angularSpeed * DEG2RAD * fixedTimeElapsedS, maxAngularSpeed);
			else if (movement.x > 0.0f)
				angularVel = b2Max(angularVel - angularSpeed * DEG2RAD * fixedTimeElapsedS, -maxAngularSpeed);
			else
				angularVel *= 0.8;

			if (movement.y > 0.0f)
			{
				linearVel.x = linearVel.x + direction.x * speed * fixedTimeElapsedS;
				linearVel.y = linearVel.y + direction.y * speed * fixedTimeElapsedS;
			}
			else if (movement.y < 0.0f)
			{
				linearVel.x = linearVel.x - direction.x * speed * fixedTimeElapsedS;
				linearVel.y = linearVel.y - direction.y * speed * fixedTimeElapsedS;
			}
			else
			{
				linearVel.x *= 0.8;
				linearVel.y *= 0.8;
			}

			auto clampMagnitude = [](const b2Vec2& v, f32 max)
			{
				auto n = v.Length();
				if (n == 0.0f) return b2Vec2(0.0f, 0.0f);
				auto f = b2Min(n, max) / n;
				return b2Vec2(f * v.x, f * v.y);
			};
			linearVel = clampMagnitude(linearVel, maxSpeed);

			physicsComponent.m_pBody->SetAngularVelocity(angularVel);
			physicsComponent.m_pBody->SetLinearVelocity(linearVel);
		}
	}
}
