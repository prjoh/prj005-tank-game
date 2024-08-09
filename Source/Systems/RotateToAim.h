#pragma once


#include <vector>

#include <System.h>

namespace pd { class RotateToAimComponent; }
namespace pd { class ActorCommand; }
namespace pd { class Transform; }


namespace pd
{
	struct RotateToAimTuple
	{
		observer_ptr<RotateToAimComponent> rotateToAim;
		observer_ptr<ActorCommand> command;
		observer_ptr<Transform> transform;
	};

	class RotateToAimSystem : public System
	{
	public:
		RotateToAimSystem(observer_ptr<EntityManager> entityManager);
		RotateToAimSystem() = delete;
		~RotateToAimSystem() = default;

	public:
		void update(f32 timeElapsedS) override;

	private:
		auto getRotateToAimTuples() -> std::vector<RotateToAimTuple>&;

	private:
		std::vector<RotateToAimTuple> m_rotateToAimTuples;
	};
}
