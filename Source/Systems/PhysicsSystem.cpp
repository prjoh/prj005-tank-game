#include <Systems/PhysicsSystem.h>


#include <algorithm>

#include <Entity.h>
#include <Utils.h>


pd::PhysicsSystem::PhysicsSystem(observer_ptr<EntityManager> entityManager)
	: System(entityManager)
{
	m_componentGroups = {
		{utils::stringHash("DynamicPhysicsTuple"), {
			typeid(DynamicPhysicsComponent),
			typeid(Transform),
		}}, 
		{utils::stringHash("StaticPhysicsTuple"), {
			typeid(StaticPhysicsComponent),
			typeid(Transform),
		}},
		//{utils::stringHash("KinematicPhysicsTuple"), {
		//	typeid(KinematicPhysicsComponent),
		//	typeid(Transform),
		//}},
	};
}

void pd::PhysicsSystem::initialize()
{
	getDynamicPhysicsTuples();  // TODO
	//getStaticPhysicsTuples();  // TODO
	//getKinematicPhysicsTuples();  // TODO
}

void pd::PhysicsSystem::fixedUpdate(f32 fixedTimeElapsed)
{
	auto* physicsWorld = m_pEntityManager->getSingletonComponent<PhysicsWorld>();
	physicsWorld->m_world.Step(physicsWorld->m_timeStep, physicsWorld->m_velocityIterations, physicsWorld->m_positionIterations);

	//for (b2Contact* contact = physicsWorld->m_world.GetContactList(); contact; contact = contact->GetNext())
	//{
	//	const auto& typeA = contact->GetFixtureA()->GetBody()->GetType();
	//	const auto& typeB = contact->GetFixtureB()->GetBody()->GetType();
	//}

	for (const auto& dt : getDynamicPhysicsTuples())
	{
		//const PhysicsComponent* dpc = dt.physicsComponent;

		//for (const auto& st : getStaticPhysicsTuples())
		//{
		//	const PhysicsComponent* spc = st.physicsComponent;
		//	if (b2TestOverlap(spc->m_pShape.get(), 0, dpc->m_pShape.get(), 0, spc->m_pBody->GetTransform(), dpc->m_pBody->GetTransform()))
		//		int x = 0;
		//}

		const auto& physicsTransform = dt.physicsComponent->m_pBody->GetTransform();
		dt.transform->position(Vector2Multiply({ physicsTransform.p.x, physicsTransform.p.y }, WORLD2SCREEN));
		dt.transform->rotation(-physicsTransform.q.GetAngle() * RAD2DEG);  // World is CW rotation / Physics Wrold is CCW rotation
	}
}

auto pd::PhysicsSystem::getDynamicPhysicsTuples() -> std::vector<DynamicPhysicsTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("DynamicPhysicsTuple"))))
		m_pEntityManager->getDynamicPhysicsTuples(m_dynamicPhysicsTuples);
	return m_dynamicPhysicsTuples;
}

//auto pd::PhysicsSystem::getStaticPhysicsTuples() -> std::vector<StaticPhysicsTuple>&
//{
//	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("StaticPhysicsTuple"))))
//		m_pEntityManager->getStaticPhysicsTuples(m_staticPhysicsTuples);
//	return m_staticPhysicsTuples;
//}

//auto pd::PhysicsSystem::getKinematicPhysicsTuples() -> std::vector<KinematicPhysicsTuple>&
//{
//	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("KinematicPhysicsTuple"))))
//		m_pEntityManager->getKinematicPhysicsTuples(m_kinematicPhysicsTuples);
//	return m_kinematicPhysicsTuples;
//}
