#pragma once


#include <vector>

#include <System.h>

namespace pd { class ParticleSystemComponent; }
namespace pd { class Transform; }


namespace pd
{
	struct ParticleSystemTuple
	{
		observer_ptr<ParticleSystemComponent> particleSystem;
		observer_ptr<Transform> transform;
	};

	class ParticleSystem : public System
	{
	public:
		ParticleSystem(observer_ptr<EntityManager> entityManager);
		ParticleSystem() = delete;
		~ParticleSystem() = default;

	public:
		void update(f32 timeElapsedS) override;

	private:
		auto getParticleSystemTuples()->std::vector<ParticleSystemTuple>&;

	private:
		std::vector<ParticleSystemTuple> m_particleSystemTuples{};
	};
}
