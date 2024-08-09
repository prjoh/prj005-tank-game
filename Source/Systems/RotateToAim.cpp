#include <Systems/RotateToAim.h>


#include <Entity.h>
#include <Types.h>
#include <Utils.h>


pd::RotateToAimSystem::RotateToAimSystem(observer_ptr<EntityManager> entityManager) : System(entityManager)
{
	m_componentGroups = {
		{utils::stringHash("RotateToAimTuple"), {
			typeid(RotateToAimComponent),
			typeid(ActorCommand),
			typeid(Transform),
		}},
	};
}

void pd::RotateToAimSystem::update(f32 timeElapsedS)
{
	for (const auto& t : getRotateToAimTuples())
	{
		Vector2 aimAt = t.command->m_aimAt;
		Vector2 position = t.transform->position();
		Vector2 direction = Vector2Subtract(aimAt, position);

		//// TODO: lerp makes 360 rotation between quadrant 3 and 4
		//f32 rotation = pd::lerp(
		//	t.transform->rotation(), 
		//	std::atan2(direction.x, -direction.y) * 180.0f / PI, 
		//	timeElapsedS * 2.0f
		//);

		// Calculate the target angle in degrees
		f32 targetAngle = std::atan2(direction.x, -direction.y) * 180.0f / PI;

		// Normalize angles to [0, 360) range
		f32 currentAngle = fmod(t.transform->rotation(), 360.0f);
		f32 angleDifference = targetAngle - currentAngle;
		if (angleDifference > 180.0f) {
			angleDifference -= 360.0f;
		}
		if (angleDifference < -180.0f) {
			angleDifference += 360.0f;
		}

		// Interpolate the angles using slerp
		f32 rotation = currentAngle + angleDifference * pd::lerp(0.0f, 1.0f, timeElapsedS * 2.0f);

		// Normalize the resulting angle
		rotation = fmod(rotation, 360.0f);
		if (rotation < 0.0f) {
			rotation += 360.0f;
		}

	    t.transform->rotation(rotation);
	}
}

auto pd::RotateToAimSystem::getRotateToAimTuples() -> std::vector<RotateToAimTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("RotateToAimTuple"))))
		m_pEntityManager->getRotateToAimTuples(m_rotateToAimTuples);
	return m_rotateToAimTuples;
}
