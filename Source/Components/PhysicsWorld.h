#pragma once


#include <box2d/b2_world.h>

#include <Component.h>
#include <Singleton.h>


namespace pd
{
#define METER2PIXEL 16.0f
#define PIXEL2METER 1.0f / 16.0f
#define WORLD2SCREEN Vector2{METER2PIXEL, -METER2PIXEL}
#define SCREEN2WORLD Vector2{PIXEL2METER, -PIXEL2METER}

	struct PhysicsWorldArgs : public ComponentArgs
	{
		Vector2 gravity = { 0.0f, 0.0f };
		f32 timeStep = 1.0f / 60.0f;
		s32 velocityIterations = 8;
		s32 positionIterations = 3;
	};

	class PhysicsWorld : public SingletonComponent, public Singleton<PhysicsWorld>
	{
		friend class Singleton<PhysicsWorld>;
		friend class EntityManager;

		friend class PhysicsSystem;
		friend class PhysicsComponent;

	private:
		PhysicsWorld() = default;
		~PhysicsWorld() = default;

	private:
		void init(ObjectArgs* pArgs) override
		{
			ASSERT_MSG(pArgs, "Expected PhysicsWorldArgs was null.");

			auto args = *reinterpret_cast<PhysicsWorldArgs*>(pArgs);

			m_timeStep = args.timeStep;
			m_velocityIterations = args.velocityIterations;
			m_positionIterations = args.positionIterations;
			m_world.SetGravity({ args.gravity.x, args.gravity.y });
		}

	private:
		f32 m_timeStep{};
		s32 m_velocityIterations{};
		s32 m_positionIterations{};
		b2World m_world{ {0.0f, 0.0f} };
	};
}
