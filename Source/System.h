#pragma once

#include <Config.h>

#if (DEBUG_BUILD)
#include <chrono>
#endif

#include <string>
#include <typeindex>
#include <unordered_map>

#include <Types.h>


namespace pd { class EntityManager; }


namespace pd
{
	class System
	{
	public:
		System(observer_ptr<EntityManager> entityManager) : m_pEntityManager(entityManager) {}
		System() = delete;
		virtual ~System() { m_pEntityManager = nullptr; }

	public:
		virtual void fixedUpdate(f32 fixedTimeElapsedS) {};
		virtual void update(f32 timeElapsedS) {};
		virtual void lateUpdate(f32 timeElapsedS) {};

		// TODO: This is ugly... we want to put all state (see Tuple management) into Singleton components!!!
		virtual void initialize() {}

#if (DEBUG_BUILD)
		void setName(const std::string& name) { m_name = name; }
		void debugUpdate(f32 timeElapsedS);
		std::string getTimeString();
#endif

	protected:
		observer_ptr<EntityManager> m_pEntityManager;

		std::unordered_map<size_t, std::vector<std::type_index>> m_componentGroups{};

#if (DEBUG_BUILD)
		std::string m_name{};
		std::chrono::steady_clock::time_point m_systemStart{};
		u64 m_timeMilli{};
		u64 m_timeMicro{};
#endif
	};
}
