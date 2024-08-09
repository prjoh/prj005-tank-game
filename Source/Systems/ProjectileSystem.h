#pragma once


#include <vector>

#include <System.h>

namespace pd { class ActorCommand; }
namespace pd { class Transform; }
namespace pd { class Projectile; }
namespace pd { class ProjectileSpawner; }


namespace pd
{
	struct ProjectileSpawnerTuple
	{
		observer_ptr<ActorCommand> command;
		observer_ptr<ProjectileSpawner> spawner;
		observer_ptr<Transform> transform;
	};

	struct ProjectileSimulationTuple
	{
		observer_ptr<Transform> transform;
		observer_ptr<Projectile> projectile;
	};

	class ProjectileSystem : public System
	{
	public:
		ProjectileSystem(observer_ptr<EntityManager> entityManager);
		ProjectileSystem() = delete;
		~ProjectileSystem() = default;

	public:
		void update(f32 timeElapsedS) override;

	private:
		auto getProjectileSpawnerTuples() -> std::vector<ProjectileSpawnerTuple>&;
		auto getProjectileSimulationTuples() -> std::vector<ProjectileSimulationTuple>&;

	private:
		std::vector<ProjectileSpawnerTuple> m_projectileSpawnerTuples;
		std::vector<ProjectileSimulationTuple> m_projectileSimulationTuples;
	};
}
