#pragma once


#include <vector>

#include <System.h>

namespace pd { class PointerAimComponent; }
namespace pd { class ActorCommand; }
namespace pd { class Transform; }


namespace pd
{
	struct PointerAimTuple
	{
		observer_ptr<PointerAimComponent> pointerAimComp;
		observer_ptr<ActorCommand> command;
		observer_ptr<Transform> transform;
	};

	class PointerAimSystem : public System
	{
	public:
		PointerAimSystem(observer_ptr<EntityManager> entityManager);
		PointerAimSystem() = delete;
		~PointerAimSystem() = default;

	public:
		void update(f32 timeElapsedS) override;

	private:
		PointerAimTuple m_pointerAimTuple{};
	};
}
