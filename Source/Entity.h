#pragma once


#include <memory>
#include <string>
#include <typeindex>
#include <type_traits>
#include <unordered_map>

#include <Guid.h>
#include <Component.h>
#include <Config.h>
#include <Entity.h>
#include <ObjectPool.h>
#include <System.h>

#include <Components/ComponentTypes.h>
#include <Components/ActorCommand.h>
#include <Components/ActorIsMoving.h>
#include <Components/ActorIsShooting.h>
#include <Components/AnimationFSM.h>
//#include <Components/FilterBits.h>
#include <Components/Graphics.h>
#include <Components/Input.h>
#include <Components/Camera.h>
#include <Components/PlayerCommand.h>
#include <Components/Transform.h>
#include <Components/PointerAim.h>
#include <Components/RotateToAim.h>
#include <Components/CameraFollow.h>
#include <Components/ParticleSystemComponent.h>
#include <Components/PhysicsComponent.h>
#include <Components/PhysicsWorld.h>
#include <Components/Projectile.h>
//#include <Components/TileMap.h>

#include <Systems/CommandSystem.h>
#include <Systems/GraphicsAnimator.h>
#include <Systems/RenderSystem.h>
#include <Systems/PlayerController.h>
#include <Systems/PointerAim.h>
#include <Systems/RotateToAim.h>
#include <Systems/CameraSystem.h>
#include <Systems/ParticleSystem.h>
#include <Systems/PhysicsSystem.h>
#include <Systems/ProjectileSystem.h>


namespace pd { class EntityManager; }


namespace pd
{
	struct EntityArgs : public ObjectArgs
	{
		observer_ptr<EntityManager> manager;
		std::string name;
	};

	class Entity : public PooledObject<ObjectPool<Entity>>
	{
		using entity_pool_type = ObjectPool<Entity>;
		using base_type = PooledObject<entity_pool_type>;
		using component_ptr = observer_ptr<Component>;

	friend class entity_pool_type;
	friend class Entity;

	public:
		Entity() = default;
		~Entity() { m_manager = nullptr; }  // TODO: Destroy comopnents

	public:
		// TODO: This is not nice ...
		void postInit();  // Called after component init phase

	protected:
		void onConstruction() override {}		 // Called when object is allocated
		void init(ObjectArgs* pArgs) override;	 // Called when object is created by pool

	public:
		void destroyComponents();

		template <class T>
		void addComponent(ComponentArgs* args = nullptr);
		template <class T>
		T* getComponent();

		//void addComponent(eComponentType type, ComponentArgs* args = nullptr);

		template <class T>
		// Returns component pointer if found on this or any parent
		T* getComponentInParent();
		template <class T>
		// Returns component pointer if found on this or any child
		T* getComponentInChildren();

		const Guid& getId() const { return m_id; }

		void setParent(Entity* parent);
		Entity* parent() const { return m_parent; }
		const std::vector<Entity*>& children() const { return m_children; }

		void findEntity();

		void registerHandle();
		void broadcast();

		std::string_view getName() const { return m_name; }

		//void update();

	private:
		void updateChildReferences();
		void updateComponents();

	private:
		Guid m_id{ Guid::Generate() };
		std::string m_name{};
		// TODO: This limits to one component type instance per entity 
		// TODO: Why do we store ptr instead of u32 (ndx)?
		std::unordered_map<std::type_index, component_ptr> m_components;

		observer_ptr<EntityManager> m_manager{ nullptr };

		Entity* m_parent{ nullptr };
		std::vector<Entity*> m_children{};
	};


	class EntityManager
	{
		using entity_ptr = observer_ptr<Entity>;
		using entity_key = size_t;

	friend class Entity;

	public:
		EntityManager()
			: m_entityPool(ENTITY_POOL_SIZE),
			  m_graphicsPool(GRAPHICS_POOL_SIZE),
			  m_transformPool(TRANSFORM_POOL_SIZE),
			  m_actorCommandPool(ACTOR_COMMAND_POOL_SIZE),
			  m_animationFSMPool(ANIMATION_FSM_POOL_SIZE),
			  m_animationStatePool(ANIMATION_STATE_POOL_SIZE),
			  m_rotateToAimPool(ROTATE_TO_AIM_POOL_SIZE),
			  m_particleSystemPool(PARTICLE_SYSTEM_POOL_SIZE),
			  m_actorIsMovingPool(ACTOR_MOVING_POOL_SIZE),
			  m_actorIsShootingPool(ACTOR_SHOOTING_POOL_SIZE),
			  m_projectileSpawnerPool(PROJECTILE_SPAWNER_POOL_SIZE),
			  m_projectilePool(PROJECTILE_POOL_SIZE),
			  m_dynamicPhysicsComponentPool(DYN_PHYSICS_COMPONENT_POOL_SIZE),
			  m_staticPhysicsComponentPool(STA_PHYSICS_COMPONENT_POOL_SIZE),
			  m_kinematicPhysicsComponentPool(KIN_PHYSICS_COMPONENT_POOL_SIZE)
			//m_tileMapPool(TILE_MAP_POOL_SIZE)
			//m_playerCommandPool(PLAYER_COMMAND_POOL_SIZE),
			//m_filterBitsPool(FILTER_BITS_POOL_SIZE),
		{
			m_componentPools[typeid(ObjectPool<Graphics>)] = &m_graphicsPool;
			m_componentPools[typeid(ObjectPool<Transform>)] = &m_transformPool;
			m_componentPools[typeid(ObjectPool<ActorCommand>)] = &m_actorCommandPool;
			m_componentPools[typeid(ObjectPool<AnimationFSM>)] = &m_animationFSMPool;
			m_componentPools[typeid(ObjectPool<AnimationState>)] = &m_animationStatePool;
			m_componentPools[typeid(ObjectPool<RotateToAimComponent>)] = &m_rotateToAimPool;
			m_componentPools[typeid(ObjectPool<ParticleSystemComponent>)] = &m_particleSystemPool;
			m_componentPools[typeid(ObjectPool<ActorIsMoving>)] = &m_actorIsMovingPool;
			m_componentPools[typeid(ObjectPool<ActorIsShooting>)] = &m_actorIsShootingPool;
			m_componentPools[typeid(ObjectPool<ProjectileSpawner>)] = &m_projectileSpawnerPool;
			m_componentPools[typeid(ObjectPool<Projectile>)] = &m_projectilePool;
			m_componentPools[typeid(ObjectPool<DynamicPhysicsComponent>)] = &m_dynamicPhysicsComponentPool;
			m_componentPools[typeid(ObjectPool<StaticPhysicsComponent>)] = &m_staticPhysicsComponentPool;
			m_componentPools[typeid(ObjectPool<KinematicPhysicsComponent>)] = &m_kinematicPhysicsComponentPool;
			//m_componentPools[typeid(ObjectPool<TileMap>)] = &m_tileMapPool;

			//m_componentPools[typeid(ObjectPool<PlayerCommand>)] = &m_playerCommandPool;
			//m_componentPools[typeid(ObjectPool<FilterBits>)] = &m_filterBitsPool;
			
			// Entity owned singletons
			m_singletonComponents[typeid(PointerAimComponent)] = &PointerAimComponent::Get();
			m_singletonComponents[typeid(CameraFollow)] = &CameraFollow::Get();

			// Global singletons
			m_singletonComponents[typeid(Input)] = &Input::Get();
			m_singletonComponents[typeid(Camera)] = &Camera::Get();
			m_singletonComponents[typeid(PhysicsWorld)] = &PhysicsWorld::Get();

			m_componentDirtyFlags = {
				{typeid(Graphics), true},
				{typeid(Transform), true},
				{typeid(ActorCommand), true},
				{typeid(AnimationFSM), true},
				{typeid(AnimationState), true},
				{typeid(RotateToAimComponent), true},
				{typeid(PointerAimComponent), true},
				{typeid(CameraFollow), true},
				{typeid(ParticleSystemComponent), true},
				{typeid(ActorIsMoving), true},
				{typeid(ActorIsShooting), true},
				{typeid(ProjectileSpawner), true},
				{typeid(Projectile), true},
				{typeid(DynamicPhysicsComponent), true},
				{typeid(StaticPhysicsComponent), true},
				{typeid(KinematicPhysicsComponent), true},
			};
		}
		~EntityManager() = default;

	public:
		Entity* createEntity(const std::string& name);
		void destroyEntity(Entity* entity);

		Entity* getEntityById(const std::string& name) const;

#if (DEBUG_BUILD)
		u32 getLiveEntites() const { return (u32)m_activeEntities.size(); }
#endif

		void registerSystem(observer_ptr<System> system) { m_systems.emplace_back(system); }

		void initialize();
		void update();

		template <class T>
		T* getSingletonComponent()
		{
			auto it = m_singletonComponents.find(typeid(T));

			if (it == m_singletonComponents.end())
				return nullptr;

			auto* component = dynamic_cast<T*>(it->second);
			return component;
		}

		void setComponentDirty(std::type_index type);
		bool hasSystemTupleUpdates(std::vector<std::type_index> tupleTypes);

		void getEntityRenderTuples(std::vector<EntityRenderTuple>& outTuples);
		void getParticlesRenderTuples(std::vector<ParticlesRenderTuple>& outTuples);
		void getCommandTuples(std::vector<CommandTuple>& outTuples);
		void getControllerTuples(std::vector<PlayerControllerTuple>& outTuples);
		void getGraphicsAnimatorTuples(std::vector<GraphicsAnimatorTuple>& outTuples);
		void getPointerAimTuple(PointerAimTuple& outTuple);
		void getRotateToAimTuples(std::vector<RotateToAimTuple>& outTuples);
		void getParticleSystemTuples(std::vector<ParticleSystemTuple>& outTuples);
		void getActorMovingTuples(std::vector<ActorIsMovingTuple>& outTuples);
		void getActorShootingTuples(std::vector<ActorIsShootingTuple>& outTuples);
		void getProjectileSpawnerTuples(std::vector<ProjectileSpawnerTuple>& outTuples);
		void getProjectileSimulationTuples(std::vector<ProjectileSimulationTuple>& outTuples);
		void getDynamicPhysicsTuples(std::vector<DynamicPhysicsTuple>& outTuples);
		void getStaticPhysicsTuples(std::vector<StaticPhysicsTuple>& outTuples);
		void getKinematicPhysicsTuples(std::vector<KinematicPhysicsTuple>& outTuples);

	private:
		template <class T, typename std::enable_if<std::is_base_of<SingletonComponent, T>::value>::type* = nullptr>
		T* createComponent(ComponentArgs* args)
		{
			auto it = m_singletonComponents.find(typeid(T));
			ASSERT_MSG(it != m_singletonComponents.end(), "Unable to find singleton component type.");
			auto component = dynamic_cast<T*>(it->second);
			component->init(args);
			return component;
		}

		template <class T, typename std::enable_if<std::is_base_of<PooledObject<ObjectPool<T>>, T>::value>::type* = nullptr>
		T* createComponent(ComponentArgs* args)
		{
			auto it = m_componentPools.find(typeid(ObjectPool<T>));
			ASSERT_MSG(it != m_componentPools.end(), "Unable to find pooled component type.");
			auto& pool = *dynamic_cast<ObjectPool<T>*>(it->second);

			auto it2 = m_componentDirtyFlags.find(typeid(T));
			ASSERT_MSG(it2 != m_componentDirtyFlags.end(), "Unable to find pooled component type.");
			it2->second = true;

			return pool.create(args);
		}

	private:
		ObjectPool<Entity> m_entityPool;
		ObjectPool<Graphics> m_graphicsPool;
		ObjectPool<Transform> m_transformPool;
		ObjectPool<ActorCommand> m_actorCommandPool;
		ObjectPool<AnimationFSM> m_animationFSMPool;
		ObjectPool<AnimationState> m_animationStatePool;
		ObjectPool<RotateToAimComponent> m_rotateToAimPool;
		ObjectPool<ParticleSystemComponent> m_particleSystemPool;
		ObjectPool<ActorIsMoving> m_actorIsMovingPool;
		ObjectPool<ActorIsShooting> m_actorIsShootingPool;
		ObjectPool<ProjectileSpawner> m_projectileSpawnerPool;
		ObjectPool<Projectile> m_projectilePool;
		ObjectPool<DynamicPhysicsComponent> m_dynamicPhysicsComponentPool;
		ObjectPool<StaticPhysicsComponent> m_staticPhysicsComponentPool;
		ObjectPool<KinematicPhysicsComponent> m_kinematicPhysicsComponentPool;

		//ObjectPool<TileMap> m_tileMapPool;
		//ObjectPool<PlayerCommand> m_playerCommandPool;
		//ObjectPool<FilterBits> m_filterBitsPool;

		std::unordered_map<std::type_index, observer_ptr<ObjectPoolBase>> m_componentPools;
		std::unordered_map<std::type_index, observer_ptr<SingletonComponent>> m_singletonComponents;

		// A component type will be marked as dirty if
		//  - Component of type was created
		//  - Component of type was destroyed
		//  - Owner of component of type has changed
		std::unordered_map<std::type_index, bool> m_componentDirtyFlags;

		std::unordered_map<std::string_view, pd::Guid, std::hash<std::string_view>> m_entityIdLUT;

		std::unordered_map<pd::Guid, observer_ptr<Entity>> m_activeEntities;
		std::vector<observer_ptr<Entity>> m_destroyEntites;
		std::vector<observer_ptr<System>> m_systems;
	};

	template<class T>
	inline void Entity::addComponent(ComponentArgs* args)
	{
		auto component = m_manager->createComponent<T>(args);
		component->acquireEntity(this);
		m_components.emplace(typeid(T), component);
	}

	template <class T>
	inline T* Entity::getComponent()
	{
		auto it = m_components.find(typeid(T));

		if (it == m_components.end())
			return nullptr;

		auto* component = dynamic_cast<T*>(it->second);
		return component;
	}

	template<class T>
	inline T* Entity::getComponentInParent()
	{
		auto component = getComponent<T>();
		if (component)
			return component;

		if (!m_parent)
			return nullptr;

		return m_parent->getComponentInParent<T>();
	}

	template<class T>
	inline T* Entity::getComponentInChildren()
	{
		auto component = getComponent<T>();
		if (component)
			return component;

		if (m_children.size() == 0)
			return nullptr;

		for (const auto& child : m_children)
		{
			auto component = child->getComponentInChildren<T>();
			if (component)
				return component;
		}

		return nullptr;
	}
}
