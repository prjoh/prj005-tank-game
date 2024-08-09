#pragma once


#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <Error.h>
#include <Types.h>


namespace pd { class Game; }


namespace pd
{
	class ServiceBase
	{
		friend class ServiceContainer;
	public:
		ServiceBase(void* instance) : m_instance(instance) {}
		ServiceBase() = delete;
		virtual ~ServiceBase() = default;

		virtual void destroy() = 0;

	protected:
		void* m_instance = nullptr;
	};

	template <class T>
	class Service final : public ServiceBase
	{
	public:
		Service(void* instance) : ServiceBase(instance) {}
		Service() = delete;

		void destroy()
		{
			if (!m_instance) return;
			delete(reinterpret_cast<T*>(m_instance));
		}
	};

	class ServiceContainer
	{
	public:
		ServiceContainer() = default;
		~ServiceContainer()
		{
			for (auto& [_, service] : m_services)
			{
				service->destroy();
			}
		}

	public:
		template<class T>
		void add(T* service)
		{
			ASSERT_MSG(m_services.find(typeid(T)) == m_services.end(), "Tried registering multiple types of the same service.");
			m_services[typeid(T)] = std::make_unique<Service<T>>(service);
		}

		template<class T>
		T& get()
		{
			auto it = m_services.find(typeid(T));
			auto service = it != m_services.cend() ? it->second.get() : nullptr;

			ASSERT_MSG(service != nullptr && service->m_instance != nullptr, "Service not found.");

			return *reinterpret_cast<T*>(service->m_instance);
		}

	private:
		std::unordered_map<std::type_index, std::unique_ptr<ServiceBase>> m_services;
	};

	class ServiceGetter
	{
		friend class Game;
	public:
		template<class T>
		static T& get()
		{
			return m_serviceContainer->get<T>();
		}

	private:
		static std::unique_ptr<ServiceContainer> m_serviceContainer;
	};
}
