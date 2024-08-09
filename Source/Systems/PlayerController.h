#pragma once


#include <vector>

#include <System.h>


namespace pd { class DynamicPhysicsComponent; }
namespace pd { class Transform; }
namespace pd { class ActorCommand; }
namespace pd { class Graphics; }


namespace pd
{
	struct PlayerControllerTuple
	{
		observer_ptr<DynamicPhysicsComponent> physicsComponent;
		observer_ptr<Transform> transform;
		observer_ptr<ActorCommand> command;
		observer_ptr<Graphics> graphics;
	};

	class PlayerController : public System
	{
	public:
		PlayerController(observer_ptr<EntityManager> entityManager);
		PlayerController() = delete;
		~PlayerController() = default;

	public:
		void initialize() override;
		void fixedUpdate(f32 fixedTimeElapsedS) override;

	private:
		auto getPlayerControllerTuples() -> std::vector<PlayerControllerTuple>&;

	private:
		std::vector<PlayerControllerTuple> m_controllerTuples;
	};
}
