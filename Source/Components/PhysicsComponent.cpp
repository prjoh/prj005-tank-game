#include <Components/PhysicsComponent.h>

#include <string_view>

#include <Entity.h>
#include <Components/PhysicsWorld.h>


namespace pd
{
	const std::unordered_map<size_t, eBodyType> g_BodyTypes = {
		{std::hash<std::string_view>{}("Static"), eBodyType::Static},
		{std::hash<std::string_view>{}("Kinematic"), eBodyType::Kinematic},
		{std::hash<std::string_view>{}("Dynamic"), eBodyType::Dynamic},
	};

	const std::unordered_map<size_t, eColliderType> g_ColliderTypes = {
		{std::hash<std::string_view>{}("Box"), eColliderType::Box},
		{std::hash<std::string_view>{}("Circle"), eColliderType::Circle},
		{std::hash<std::string_view>{}("Polygon"), eColliderType::Polygon},
		{std::hash<std::string_view>{}("Edge"), eColliderType::Edge},
		{std::hash<std::string_view>{}("Chain"), eColliderType::Chain},
	};
	
	void PhysicsComponent::init(ObjectArgs* pArgs)	 // Called when object is created by pool
	{
		m_pWorld = &pd::PhysicsWorld::Get().m_world;

		ASSERT_MSG(m_pWorld, "PhysicsWorld could not be referenced. Did you forget to initialize one?");

		ASSERT_MSG(pArgs, "Expected PhysicsComponentArgs was null.");

		auto args = *reinterpret_cast<PhysicsComponentArgs*>(pArgs);

		// TODO: Give option to pass in a bodyPtr? For connected fixtures!
		b2BodyDef bodyDef;
		bodyDef.type = getBodyType(args.bodyType);
		bodyDef.position = { 0.0f, 0.0f };
		bodyDef.angle = 0.0f;
		bodyDef.linearDamping = args.linearDamping;
		bodyDef.angularDamping = args.angularDamping;

		m_pBody = m_pWorld->CreateBody(&bodyDef);

		b2FixtureDef fixtureDef;
		/*
		The fixture density is used to compute the mass properties of the parent body.
		The density can be zero or positive.
		You should generally use similar densities for all your fixtures. This will improve stacking stability.
		*/
		fixtureDef.density = args.density;    // (kg/m^2)
		fixtureDef.friction = args.friction;  // [0.0, 1.0]
		/*
		Restitution is used to make objects bounce. The restitution value is usually set to be between 0 and 1.
		Consider dropping a ball on a table. A value of zero means the ball won't bounce.
		This is called an inelastic collision. A value of one means the ball's velocity will be exactly reflected.
		This is called a perfectly elastic collision. Restitution is combined using the following formula.
		*/
		fixtureDef.restitution = args.restitution;  // [0.0, 1.0]

		switch (args.colliderType)
		{
		case eColliderType::Box:
		{
			createBoxCollider(pArgs, fixtureDef);
			break;
		}
		case eColliderType::Circle:
		{
			createCircleCollider(pArgs, fixtureDef);
			break;
		}
		case eColliderType::Polygon:
		{
			break;
		}
		case eColliderType::Edge:
		{
			break;
		}
		case eColliderType::Chain:
		{
			break;
		}
		default:
			ASSERT_MSG(false, "Illegal eColliderType passed.");
		}
	}

	void PhysicsComponent::postInit(Entity& owner)
	{
		const auto* transform = owner.getComponent<Transform>();

		ASSERT_MSG(transform, "PhysicsComponent requires a Transform on the same Entity!");

		const auto& position = Vector2Multiply(transform->position(), SCREEN2WORLD);
		const auto& rotation = transform->rotation() * DEG2RAD;

		m_pBody->SetTransform(
			{ position.x, position.y },
			-rotation  // World is CW rotation / Physics Wrold is CCW rotation
		);
	}

	b2BodyType PhysicsComponent::getBodyType(const eBodyType& type) const
	{
		switch (type)
		{
		case eBodyType::Static:
			return b2BodyType::b2_staticBody;
		case eBodyType::Kinematic:
			return b2BodyType::b2_kinematicBody;
		case eBodyType::Dynamic:
			return b2BodyType::b2_dynamicBody;
		default:
			ASSERT_MSG(false, "Illegal eBodyType passed.");
		}
	}

	void PhysicsComponent::createBoxCollider(ObjectArgs* pArgs, b2FixtureDef& fixtureDef)
	{
		ASSERT_MSG(pArgs, "Expected PhysicsComponentArgs was null.");
		auto args = *reinterpret_cast<BoxPhysicsComponentArgs*>(pArgs);

		m_pShape = std::make_unique<b2PolygonShape>();

		auto* pShape = static_cast<b2PolygonShape*>(m_pShape.get());
		pShape->SetAsBox(args.width * 0.5f, args.height * 0.5f);

		fixtureDef.shape = pShape;

		m_pFixture = m_pBody->CreateFixture(&fixtureDef);
	}

	void PhysicsComponent::createCircleCollider(ObjectArgs* pArgs, b2FixtureDef& fixtureDef)
	{
		ASSERT_MSG(pArgs, "Expected SpherePhysicsComponentArgs was null.");
		auto args = *reinterpret_cast<CirclePhysicsComponentArgs*>(pArgs);

		m_pShape = std::make_unique<b2CircleShape>();

		auto* pShape = static_cast<b2CircleShape*>(m_pShape.get());
		pShape->m_radius = args.radius;

		fixtureDef.shape = pShape;

		m_pFixture = m_pBody->CreateFixture(&fixtureDef);
	}
}
