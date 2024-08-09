#pragma once


#include <Component.h>


namespace pd
{
	struct ActorIsMovingArgs : public ComponentArgs
	{
	};

	class ActorIsMoving : public PooledComponent<ActorIsMoving>
	{
		using base_type = PooledComponent<ActorIsMoving>;
		using pool_type = ObjectPool<ActorIsMoving>;

		friend class pool_type;
		friend class CommandSystem;

	public:
		ActorIsMoving() = default;
		~ActorIsMoving() = default;

	private:
		void onConstruction() override {}		  // Called when object is allocated
		void init(ObjectArgs* pArgs) override     // Called when object is created by pool
		{
			m_isMoving = false;
		}

	private:
		bool m_isMoving{};
	};
}
