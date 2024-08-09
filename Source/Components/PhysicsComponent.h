#pragma once

#include <memory>
#include <unordered_map>

#include <box2d/box2d.h>

#include <Component.h>
#include <MathExtension.h>


namespace pd
{
	enum class eBodyType : u8
	{
		Static,
		Kinematic,
		Dynamic,
	};
	extern const std::unordered_map<size_t, eBodyType> g_BodyTypes;

	enum class eColliderType : u8
	{
		Box,
		Circle,
		Polygon,
		Edge,
		Chain,
	};
	extern const std::unordered_map<size_t, eColliderType> g_ColliderTypes;

	struct PhysicsComponentArgs : public ComponentArgs
	{
		eBodyType bodyType;
		eColliderType colliderType;
		f32 density;
		f32 friction;
		f32 restitution;
		f32 linearDamping = 0.0f;
		f32 angularDamping = 0.01f;
	};

	struct BoxPhysicsComponentArgs : public PhysicsComponentArgs
	{
		f32 width = 1.0f;
		f32 height = 1.0f;
	};

	struct CirclePhysicsComponentArgs : public PhysicsComponentArgs
	{
		f32 radius = 1.0f;
	};

	class PhysicsComponent
	{
		friend class PhysicsSystem;
		friend class PlayerController;
		friend class RenderSystem;

	protected:
		void init(ObjectArgs* pArgs);
		void postInit(Entity& owner);

		b2BodyType getBodyType(const eBodyType& type) const;
		void createBoxCollider(ObjectArgs* pArgs, b2FixtureDef& fixtureDef);
		void createCircleCollider(ObjectArgs* pArgs, b2FixtureDef& fixtureDef);

	protected:
		std::unique_ptr<b2Shape> m_pShape{ nullptr };

		observer_ptr<b2World> m_pWorld{ nullptr };

		observer_ptr<b2Body> m_pBody{ nullptr };
		observer_ptr<b2Fixture> m_pFixture{ nullptr };
	};

	template <class T>
	class PhysicsBaseComponent : public PooledComponent<T>, public PhysicsComponent
	{
		using base_type = PooledComponent<T>;
		using object_pool_type = ObjectPool<T>;

		friend class object_pool_type;
		friend class EntityManager;

	public:
		PhysicsBaseComponent() = default;
		~PhysicsBaseComponent() = default;

	private:
		void onConstruction() override {}		 // Called when object is allocated
		void init(ObjectArgs* pArgs) override	 // Called when object is created by pool
		{
			PhysicsComponent::init(pArgs);
		}
		void postInit() override
		{
			PhysicsComponent::postInit(*m_owner);
		}
	};

	class DynamicPhysicsComponent : public PhysicsBaseComponent<DynamicPhysicsComponent>{};
	class KinematicPhysicsComponent : public PhysicsBaseComponent<KinematicPhysicsComponent> {};
	class StaticPhysicsComponent : public PhysicsBaseComponent<StaticPhysicsComponent> {};
}
