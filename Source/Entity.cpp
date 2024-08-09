#include <Entity.h>


#include <Error.h>


using namespace pd;


void Entity::init(ObjectArgs* pArgs)
{
	auto& args = *reinterpret_cast<EntityArgs*>(pArgs);
	m_manager = args.manager;
	m_name = args.name;
}

void pd::Entity::postInit()
{
	for (auto& [_, pComponent] : m_components)
		pComponent->postInit();
}

void Entity::destroyComponents()
{
	for (auto& [type, pComponent] : m_components)
	{
		m_manager->setComponentDirty(type);
		pComponent->destroy();
	}
}

void Entity::setParent(Entity* parent)
{
	// Update child references of previous parent
	if (m_parent != nullptr) {
		m_parent->updateChildReferences();
	}

	// Update parent
	m_parent = parent;

	// Update new parent children
	if (m_parent) {
		m_parent->m_children.emplace_back(this);
	}

	// Process entity relation update on components
	updateComponents();
}

void Entity::updateChildReferences()
{
	m_children.erase(std::remove_if(m_children.begin(), m_children.end(),
		[&](Entity* child) { return child->m_parent != this; }), m_children.end());
}

void Entity::updateComponents()
{
	for (const auto& [_, component] : m_components)
		component->onEntityRelationUpdate();

	for (const auto& entity : m_children)
	{
		for (const auto& [_, component] : entity->m_components)
			component->onEntityRelationUpdate();
	}
}

Entity* EntityManager::createEntity(const std::string& name)
{
	ASSERT_MSG(m_entityIdLUT.find(name) == m_entityIdLUT.end(), "Duplicate name '{}' found for entity!", name);

	EntityArgs args;
	args.manager = this;
	args.name = name;

	auto entity = m_entityPool.create(&args);
	auto guid = entity->getId();

	m_entityIdLUT.emplace(entity->getName(), guid);

	auto key = std::hash<std::string_view>{}(guid.toString());
	ASSERT(m_activeEntities.find(guid) == m_activeEntities.end());
	m_activeEntities[guid] = entity;

	return entity;
}

void EntityManager::destroyEntity(Entity* entity)
{
	m_entityIdLUT.erase(entity->getName());
	m_destroyEntites.emplace_back(entity);
}

Entity* pd::EntityManager::getEntityById(const std::string& name) const
{
	auto it = m_entityIdLUT.find(name);
	if (it == m_entityIdLUT.end())
		return nullptr;

	return m_activeEntities.at(it->second);
}

void pd::EntityManager::initialize()
{
	for (auto& system : m_systems)
	{
		system->initialize();
	}
}

void EntityManager::update()
{
	static auto accumulatedTime = 0.0f;
	const auto& timeElapsedS = GetFrameTime();

	accumulatedTime += timeElapsedS;

	s32 fixedSteps = 0;
	while (accumulatedTime >= PHYSICS_TIMESTEP) {
		accumulatedTime -= PHYSICS_TIMESTEP;
		++fixedSteps;
	}

	for (auto& system : m_systems)
	{
		for (s32 i = 0; i < fixedSteps; ++i)
			system->fixedUpdate(PHYSICS_TIMESTEP);

#if (DEBUG_BUILD)
		system->debugUpdate(timeElapsedS);
#else
		system->update(timeElapsedS);
#endif
	}

	// Reset all component dirty flags
	for (auto& [key, _] : m_componentDirtyFlags)
		m_componentDirtyFlags[key] = false;

	// Handle entity / component lifetimes
	for (const auto entity : m_destroyEntites)
	{
		auto guid = entity->getId();
		auto key = std::hash<std::string_view>{}(guid.toString());
		ASSERT(m_activeEntities.find(guid) != m_activeEntities.end());
		m_activeEntities.erase(guid);

		entity->destroyComponents();

		m_entityPool.destroy(entity);
	}

	m_destroyEntites.clear();
}

void pd::EntityManager::setComponentDirty(std::type_index type)
{
	ASSERT_MSG(m_componentDirtyFlags.find(type) != m_componentDirtyFlags.end(), "Unknown component type was passed.");
	m_componentDirtyFlags[type] = true;
}

bool pd::EntityManager::hasSystemTupleUpdates(std::vector<std::type_index> tupleTypes)
{
	for (const auto& type : tupleTypes)
	{
		auto it = m_componentDirtyFlags.find(type);
		ASSERT_MSG(it != m_componentDirtyFlags.end(), "Illegal component type_index passed!");
		if (it->second)
			return true;
	}

	return false;
}

// TODO: Make these into one function, the user is supposed to pass vector of type_index? Maybe this is not a good idea...
void EntityManager::getEntityRenderTuples(std::vector<EntityRenderTuple>& outTuples)
{
	outTuples.clear();

	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* graphics = entity->getComponent<Graphics>();
		auto* transform = entity->getComponent<Transform>();

		if (!graphics || !transform)
			continue;

		outTuples.emplace_back(EntityRenderTuple{ transform, graphics });
	}
}

void pd::EntityManager::getParticlesRenderTuples(std::vector<ParticlesRenderTuple>& outTuples)
{
	outTuples.clear();

	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* particleSystem = entity->getComponent<ParticleSystemComponent>();
		auto* transform = entity->getComponent<Transform>();

		if (!particleSystem || !transform)
			continue;

		outTuples.emplace_back(ParticlesRenderTuple{ particleSystem, transform });
	}
}

void EntityManager::getCommandTuples(std::vector<CommandTuple>& outTuples)
{
	outTuples.clear();

	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* command = entity->getComponent<ActorCommand>();

		if (!command)
			continue;

		outTuples.emplace_back(CommandTuple{ command });
	}
}

void pd::EntityManager::getControllerTuples(std::vector<PlayerControllerTuple>& outTuples)
{
	outTuples.clear();

	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* physicsComponent = entity->getComponent<DynamicPhysicsComponent>();
		auto* transform = entity->getComponent<Transform>();
		auto* command = entity->getComponent<ActorCommand>();
		auto* graphics = entity->getComponent<Graphics>();

		if (!physicsComponent || !transform || !command || !graphics)
			continue;

		outTuples.emplace_back(PlayerControllerTuple{ physicsComponent, transform, command, graphics });
	}
}

void pd::EntityManager::getGraphicsAnimatorTuples(std::vector<GraphicsAnimatorTuple>& outTuples)
{
	outTuples.clear();

	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* animationFSM = entity->getComponent<AnimationFSM>();
		auto* graphics = entity->getComponent<Graphics>();
		auto* command = entity->getComponent<ActorCommand>();

		if (!animationFSM || !command || !graphics)
			continue;

		outTuples.emplace_back(GraphicsAnimatorTuple{ animationFSM, command, graphics });
	}
}

void pd::EntityManager::getPointerAimTuple(PointerAimTuple& outTuple)
{
	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* pointer = entity->getComponent<PointerAimComponent>();
		auto* command = entity->getComponent<ActorCommand>();
		auto* transform = entity->getComponent<Transform>();

		if (!pointer || !command || !transform)
			continue;

		outTuple.pointerAimComp = pointer;
		outTuple.command = command;
		outTuple.transform = transform;
		return;
	}
}

void pd::EntityManager::getRotateToAimTuples(std::vector<RotateToAimTuple>& outTuples)
{
	outTuples.clear();

	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* rotateToAim = entity->getComponent<RotateToAimComponent>();
		auto* command = entity->getComponent<ActorCommand>();
		auto* transform = entity->getComponent<Transform>();

		if (!rotateToAim || !command || !transform)
			continue;

		outTuples.emplace_back(RotateToAimTuple{ rotateToAim, command, transform });
	}
}

void pd::EntityManager::getParticleSystemTuples(std::vector<ParticleSystemTuple>& outTuples)
{
	outTuples.clear();

	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* particleSystem = entity->getComponent<ParticleSystemComponent>();
		auto* transform = entity->getComponent<Transform>();

		if (!particleSystem || !transform)
			continue;

		outTuples.emplace_back(ParticleSystemTuple{ particleSystem, transform });
	}
}

void pd::EntityManager::getActorMovingTuples(std::vector<ActorIsMovingTuple>& outTuples)
{
	outTuples.clear();

	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* actorMoving = entity->getComponent<ActorIsMoving>();
		auto* particleSystem = entity->getComponent<ParticleSystemComponent>();

		if (!particleSystem || !actorMoving)
			continue;

		outTuples.emplace_back(ActorIsMovingTuple{ actorMoving, particleSystem });
	}
}

void pd::EntityManager::getActorShootingTuples(std::vector<ActorIsShootingTuple>& outTuples)
{
	outTuples.clear();

	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* actorShooting = entity->getComponent<ActorIsShooting>();
		auto* particleSystem = entity->getComponent<ParticleSystemComponent>();

		if (!particleSystem || !actorShooting)
			continue;

		outTuples.emplace_back(ActorIsShootingTuple{ actorShooting, particleSystem });
	}
}

void pd::EntityManager::getProjectileSpawnerTuples(std::vector<ProjectileSpawnerTuple>& outTuples)
{
	outTuples.clear();

	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* command = entity->getComponent<ActorCommand>();
		auto* spawner = entity->getComponent<ProjectileSpawner>();
		auto* transform = entity->getComponent<Transform>();

		if (!command || !spawner || !transform)
			continue;

		outTuples.emplace_back(ProjectileSpawnerTuple{ command, spawner, transform });
	}
}

void pd::EntityManager::getProjectileSimulationTuples(std::vector<ProjectileSimulationTuple>& outTuples)
{
	outTuples.clear();

	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* projectile = entity->getComponent<Projectile>();
		auto* transform = entity->getComponent<Transform>();

		if (!projectile || !transform)
			continue;

		outTuples.emplace_back(ProjectileSimulationTuple{ transform, projectile });
	}
}

void pd::EntityManager::getDynamicPhysicsTuples(std::vector<DynamicPhysicsTuple>& outTuples)
{
	outTuples.clear();

	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* physicsComponent = entity->getComponent<DynamicPhysicsComponent>();
		auto* transform = entity->getComponent<Transform>();

		if (!physicsComponent || !transform)
			continue;

		outTuples.emplace_back(DynamicPhysicsTuple{ physicsComponent, transform });
	}
}

void pd::EntityManager::getStaticPhysicsTuples(std::vector<StaticPhysicsTuple>& outTuples)
{
	outTuples.clear();

	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* physicsComponent = entity->getComponent<StaticPhysicsComponent>();
		auto* transform = entity->getComponent<Transform>();

		if (!physicsComponent || !transform)
			continue;

		outTuples.emplace_back(StaticPhysicsTuple{ physicsComponent, transform });
	}
}

void pd::EntityManager::getKinematicPhysicsTuples(std::vector<KinematicPhysicsTuple>& outTuples)
{
	outTuples.clear();

	for (const auto& [_, entity] : m_activeEntities)
	{
		auto* physicsComponent = entity->getComponent<KinematicPhysicsComponent>();
		auto* transform = entity->getComponent<Transform>();

		if (!physicsComponent || !transform)
			continue;

		outTuples.emplace_back(KinematicPhysicsTuple{ physicsComponent, transform });
	}
}

//// TODO: This is not nice
//void pd::EntityManager::getPlayerCommand(PlayerCommandTuple& outTuple)
//{
//	outTuple.command = nullptr;
//
//	for (const auto& [_, entity] : m_activeEntities)
//	{
//		auto* command = entity->getComponent<PlayerCommand>();
//
//		if (!command)
//			continue;
//
//		outTuple.command = command;
//		break;
//	}
//}
