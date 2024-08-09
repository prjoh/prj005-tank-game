#include <Systems/CameraSystem.h>


#include <Entity.h>


void pd::CameraSystem::update(f32 timeElapsedS)
{
	auto* camera = m_pEntityManager->getSingletonComponent<Camera>();
	auto* cameraFollow = m_pEntityManager->getSingletonComponent<CameraFollow>();

	// TODO: Warning message
	if (!camera || !cameraFollow)
		return;

	auto* transform = cameraFollow->m_owner->getComponent<Transform>();

	// TODO: Warning message
	if (!transform)
		return;

	Vector2 cameraPosition = camera->position();
	// TODO: It will be better if we use the reticle position to push the camera view a little further
	Vector2 goalPosition = Vector2Add(transform->position(), Vector2Scale(transform->front(), 256.0f));
	cameraPosition = pd::vector2Lerp(cameraPosition, goalPosition, timeElapsedS);
	camera->setPosition(cameraPosition);
}
