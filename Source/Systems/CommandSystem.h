#pragma once


#include <vector>

#include <System.h>


namespace pd { class ActorCommand; }
namespace pd { class ActorIsMoving; }
namespace pd { class ActorIsShooting; }
namespace pd { class ParticleSystemComponent; }


namespace pd
{
	struct CommandTuple
	{
		observer_ptr<ActorCommand> command;
	};

	struct ActorIsMovingTuple
	{
		observer_ptr<ActorIsMoving> movingState;
		observer_ptr<ParticleSystemComponent> particleSystem;
	};

	struct ActorIsShootingTuple
	{
		observer_ptr<ActorIsShooting> shootingState;
		observer_ptr<ParticleSystemComponent> particleSystem;
	};

	class CommandSystem : public System
	{
	public:
		CommandSystem(observer_ptr<EntityManager> entityManager);
		CommandSystem() = delete;
		~CommandSystem() = default;

	public:
		void update(f32 timeElapsedS) override;

	private:
		auto getCommandTuples() -> std::vector<CommandTuple>&;
		auto getActorIsMovingTuples() -> std::vector<ActorIsMovingTuple>&;
		auto getActorIsShootingTuples() -> std::vector<ActorIsShootingTuple>&;

	private:
		std::vector<CommandTuple> m_commandTuples;
		std::vector<ActorIsMovingTuple> m_actorMovingTuples;
		std::vector<ActorIsShootingTuple> m_actorShootingTuples;
	};
}
