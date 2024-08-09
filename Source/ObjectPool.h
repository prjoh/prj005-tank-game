#pragma once


#include <vector>

#include <Error.h>
#include <Types.h>


namespace pd
{
    using object_handle = u32;

    struct ObjectArgs{};

	template <typename ObjectPoolType>
	class PooledObject
	{
        friend ObjectPoolType;
    public:
        PooledObject() = default;
        virtual ~PooledObject() { m_pObjectPool = nullptr; }

    protected:
        // TODO: This method is meaningless because of ctor
        virtual void onConstruction() = 0;          // Called when object is allocated
        virtual void init(ObjectArgs* pArgs) = 0;   // Called when object is created by pool

    //public:
    //    virtual void destroy() { m_pObjectPool->destroy(m_handle); }
    public:
        object_handle getHandle() { return m_handle; }

    private:
        void construct(object_handle handle, ObjectPoolType* pool)
        {
            m_handle = handle;
            m_pObjectPool = pool;
            m_next = (object_handle)pool->getPoolSize();
        }

        object_handle getNext() const { return m_next; }
        void setNext(object_handle next) { m_next = next; }

    protected:
        observer_ptr<ObjectPoolType> m_pObjectPool = nullptr;

    private:
        object_handle m_handle{};
        object_handle m_next{};

        bool m_isActive{ false };
	};

    class ObjectPoolBase
    {
    public:
        virtual ~ObjectPoolBase() = default;
    };

    template <typename T>
    class ObjectPool : public ObjectPoolBase
    {
    public:
        ObjectPool(/*observer_ptr<ObjectManager> manager, */size_t size);
        ObjectPool() = delete;
        virtual ~ObjectPool() = default;

        T* create(ObjectArgs* pArgs);
        //template <typename... Args>
        //const Guid& create(Args&&... args);
        //void destroy(const Guid& guid);
        void destroy(const object_handle handle);
        void destroy(T* object);

        void clear();

        //u32 activeSize() const { return m_numActive; }
        size_t getPoolSize() const { return m_poolSize; }

        //T* getObjectByGuid(const Guid& guid);
        T* getObjectByHandle(const object_handle handle);

    private:
        object_handle m_availableObject;
        u32 m_numActive;

        size_t m_poolSize;
        std::vector<T> m_objects;  // TODO: Make it std::array!
        // Maybe forget about all this. std::array will be best!!
        // TODO: We should also allow objects without default constructors!!! I don't know......
        //   std::vector<A> v1(10, 42);  // 10 elements each with value 42
        //   std::vector<A> v2{1, 2, 3, 4}; // 4 elements with different values 
        //   https://stackoverflow.com/questions/38255040/class-with-stdarray-of-objects-without-default-constructors

        //std::unordered_map<size_t, u32> m_guidToObjectMap;
    };

    template<typename T>
    inline ObjectPool<T>::ObjectPool(/*observer_ptr<ObjectManager> poolManager, */size_t size)
        : /*ObjectPoolBase(), */m_availableObject(0), m_numActive(0), m_poolSize(size), m_objects({size})
    {
        for (u32 i = 0; i < m_poolSize; ++i)
        {
            m_objects[i].construct(i, this);
            m_objects[i].onConstruction();

            //auto guid = m_objects[i].getGuid().toString();
            //auto guidKey = std::hash<std::string_view>{}(std::string_view(guid));
            //m_guidToObjectMap.insert({ guidKey, i });

            m_objects[i].setNext(i + 1);
        }
    }

    template<typename T>
    inline T* ObjectPool<T>::create(ObjectArgs* pArgs)
    {
        ASSERT_MSG(m_availableObject < m_poolSize, "Overflow when creating pooled object. Increase size.");

        m_numActive += 1;

        auto object = &m_objects[m_availableObject];

        m_availableObject = object->getNext();

        object->m_isActive = true;
        object->init(pArgs);

        return object;
    }

    //template<typename T>
    //template<typename ...Args>
    //inline const Guid& ObjectPool<T>::create(Args && ...args)
    //{
    //    // TODO
    //    //// Check if the create method is implemented in T
    //    //static_assert(std::is_same<decltype(std::declval<T>().create(std::forward<Args>(args)...)), void>::value,
    //    //              "create method not implemented in the pooled object type");


    //    PDT_ASSERT_MSG(m_availableObject < m_poolSize, "[ObjectPool] Overflow when creating pooled object. Increase size.");

    //    m_numActive += 1;

    //    auto& object = m_objects[m_availableObject];

    //    m_availableObject = object.getNext();

    //    object.create(std::forward<Args>(args)...);
    //    object.m_isActive = true;

    //    return object.getGuid();
    //}

    //template<typename T>
    //inline void ObjectPool<T>::destroy(const Guid& guid)
    //{
    //    auto object = getObjectByGuid(guid);
    //    PDT_ASSERT_MSG(object != nullptr, "[ObjectPool] Failed to destroy object.");

    //    // TODO: Duplicate
    //    auto ndx = object->getHandle();
    //    object->setNext(m_availableObject);
    //    object->m_isActive = false;

    //    m_availableObject = ndx;
    //    m_numActive -= 1;
    //}

    template<typename T>
    inline void ObjectPool<T>::destroy(const object_handle handle)
    {
        auto object = getObjectByHandle(handle);

        if (!object)
        {
            //LOG_ERROR("Unable to destroy object.")  // TODO
            return;
        }

        destroy(object);
    }

    template<typename T>
    inline void ObjectPool<T>::destroy(T* object)
    {
        object->setNext(m_availableObject);
        object->m_isActive = false;

        m_availableObject = object->m_handle;
        m_numActive -= 1;
    }

    template<typename T>
    inline void ObjectPool<T>::clear()
    {
        m_numActive = 0;
        m_availableObject = 0;

        for (u32 i = 0; i < m_poolSize; ++i)
            m_objects[i].setNext(i + 1);
    }

    //template<typename T>
    //inline T* ObjectPool<T>::getObjectByGuid(const Guid& guid)
    //{
    //    auto key = std::hash<std::string_view>{}(std::string_view(guid.toString()));

    //    // TODO: assert?
    //    auto it = m_guidToObjectMap.find(key);
    //    if (it == m_guidToObjectMap.end())
    //        return nullptr;

    //    return &m_objects[it->second];
    //}

    template<typename T>
    inline T* ObjectPool<T>::getObjectByHandle(const object_handle handle)
    {
        if (handle >= m_poolSize)
        {
            //LOG_ERROR("Illegal handle passed.")  // TODO
            return nullptr;
        }

        return &m_objects[handle];
    }
}
