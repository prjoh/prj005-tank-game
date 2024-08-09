#pragma once


#include <vector>

#include <System.h>


namespace pd { class DynamicPhysicsComponent; }
namespace pd { class StaticPhysicsComponent; }
namespace pd { class KinematicPhysicsComponent; }
namespace pd { class Transform; }

namespace pd
{
	struct DynamicPhysicsTuple
	{
		observer_ptr<DynamicPhysicsComponent> physicsComponent;
		observer_ptr<Transform> transform;
	};

	struct StaticPhysicsTuple
	{
		observer_ptr<StaticPhysicsComponent> physicsComponent;
		observer_ptr<Transform> transform;
	};

	struct KinematicPhysicsTuple
	{
		observer_ptr<KinematicPhysicsComponent> physicsComponent;
		observer_ptr<Transform> transform;
	};

	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem(observer_ptr<EntityManager> entityManager);
		PhysicsSystem() = delete;
		~PhysicsSystem() = default;

	public:
		void initialize() override;
		void fixedUpdate(f32 fixedTimeElapsedS) override;

	private:
		auto getDynamicPhysicsTuples() -> std::vector<DynamicPhysicsTuple>&;
		//auto getStaticPhysicsTuples() -> std::vector<StaticPhysicsTuple>&;
		//auto getKinematicPhysicsTuples() -> std::vector<KinematicPhysicsTuple>&;

	private:
		std::vector<DynamicPhysicsTuple> m_dynamicPhysicsTuples;
		std::vector<StaticPhysicsTuple> m_staticPhysicsTuples;
		std::vector<KinematicPhysicsTuple> m_kinematicPhysicsTuples;
	};
}
