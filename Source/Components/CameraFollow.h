#pragma once


#include <Component.h>
#include <Singleton.h>


namespace pd
{
	struct CameraFollowArgs : public ComponentArgs
	{
	};

	class CameraFollow: public SingletonComponent, public Singleton<CameraFollow>
	{
		friend class Singleton<CameraFollow>;
		friend class EntityManager;

		friend class CameraSystem;

	private:
		CameraFollow() = default;
		~CameraFollow() = default;

	private:
		void init(ObjectArgs* pArgs) override {}
	};
}
