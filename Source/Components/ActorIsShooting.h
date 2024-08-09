#pragma once


#include <Component.h>


namespace pd
{
	struct ActorIsShootingArgs : public ComponentArgs
	{
	};

	class ActorIsShooting : public PooledComponent<ActorIsShooting>
	{
		using base_type = PooledComponent<ActorIsShooting>;
		using pool_type = ObjectPool<ActorIsShooting>;

		friend class pool_type;
		friend class CommandSystem;

	public:
		ActorIsShooting() = default;
		~ActorIsShooting() = default;

	private:
		void onConstruction() override {}		  // Called when object is allocated
		void init(ObjectArgs* pArgs) override     // Called when object is created by pool
		{
			m_isShooting = false;
		}

	private:
		bool m_isShooting{};
	};
}
