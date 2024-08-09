#pragma once


#include <ObjectPool.h>
#include <Types.h>


namespace pd { class Entity; }


namespace pd
{
	struct ComponentArgs : public ObjectArgs
	{
	};

	class Component : public NonMovable
	{
		friend class Entity;
	public:
		Component() = default;
		virtual ~Component() { m_owner = nullptr; };

	public:
		virtual void destroy() = 0;

	protected:
		void acquireEntity(Entity* owner) { m_owner = owner; }
		virtual void postInit() = 0;  // Called after component init phase

	//public:
	//	virtual void update(f32 timeElapsedS) = 0;

	private:
		virtual void onEntityRelationUpdate() {}

	protected:
		observer_ptr<Entity> m_owner{ nullptr };
	};

	template <class T>
	class PooledComponent : public Component, public PooledObject<ObjectPool<T>>
	{
		using component_pool_type = ObjectPool<T>;
		using base_type = PooledObject<component_pool_type>;

	friend class component_pool_type;
	friend class Entity;

	public:
		PooledComponent() = default;
		virtual ~PooledComponent() = default;

	public:
		void destroy() override { this->m_pObjectPool->destroy(this->getHandle()); }

	protected:
		void init(ObjectArgs* pArgs) override {}
		void postInit() override {}  // Called after component init phase
	};

	class SingletonComponent : public Component
	{
		friend class EntityManager;
	public:
		SingletonComponent() = default;
		virtual ~SingletonComponent() = default;

	public:
		void destroy() override { ASSERT_MSG(false, "You cannot destroy singleton components."); }

	protected:
		virtual void init(ObjectArgs* pArgs) = 0;
		void postInit() override {}  // Called after component init phase
	};
}
