#include <Systems/PointerAim.h>


#include <Entity.h>
#include <Utils.h>


pd::PointerAimSystem::PointerAimSystem(observer_ptr<EntityManager> entityManager) : System(entityManager)
{
	m_componentGroups = {
		{utils::stringHash("PointerAimTuple"), {
			typeid(PointerAimComponent),
			typeid(ActorCommand),
			typeid(Transform),
		}},
	};
}

void pd::PointerAimSystem::update(f32 timeElapsedS)
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("PointerAimTuple"))))
		m_pEntityManager->getPointerAimTuple(m_pointerAimTuple);
	
	auto& t = m_pointerAimTuple;
	if (!t.pointerAimComp || !t.command || !t.transform)
		return;

	t.transform->position(t.command->m_aimAt);
}
