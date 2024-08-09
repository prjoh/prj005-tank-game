#pragma once


#include <System.h>


namespace pd
{
	struct CameraTuple
	{
	};

	class CameraSystem : public System
	{
	public:
		CameraSystem(observer_ptr<EntityManager> entityManager) : System(entityManager) {}
		CameraSystem() = delete;
		~CameraSystem() = default;

	public:
		void update(f32 timeElapsedS) override;
	};
}
