#pragma once


#include <Component.h>


namespace pd
{
	struct RotateToAimArgs : public ComponentArgs
	{
	};

	class RotateToAimComponent : public PooledComponent<RotateToAimComponent>
	{
		using base_type = PooledComponent<RotateToAimComponent>;
		using pool_type = ObjectPool<RotateToAimComponent>;

		friend class pool_type;

	public:
		RotateToAimComponent() = default;
		~RotateToAimComponent() = default;

	private:
		void onConstruction() override {}		  // Called when object is allocated
		void init(ObjectArgs* pArgs) override {}  // Called when object is created by pool
	};
}
