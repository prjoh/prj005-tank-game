#pragma once


#include <Component.h>
#include <Singleton.h>


namespace pd
{
	struct PointerAimArgs : public ComponentArgs
	{
	};

	class PointerAimComponent : public SingletonComponent, public Singleton<PointerAimComponent>
	{
		friend class Singleton<PointerAimComponent>;
		friend class EntityManager;

	private:
		PointerAimComponent() = default;
		~PointerAimComponent() = default;

	private:
		void init(ObjectArgs* pArgs) override {}
	};
}
