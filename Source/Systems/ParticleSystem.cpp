#include <Systems/ParticleSystem.h>


#include <Entity.h>
#include <Utils.h>


pd::ParticleSystem::ParticleSystem(observer_ptr<EntityManager> entityManager)
	: System(entityManager)
{
	m_componentGroups = {
		{utils::stringHash("ParticleTuple"), {
			typeid(ParticleSystemComponent),
			typeid(Transform),
		}},
	};
}

void pd::ParticleSystem::update(f32 timeElapsedS)
{
	// TODO: Iterate over different tuple configurations to set PS->m_isPlaying

	for (const auto& t : getParticleSystemTuples())
	{
		const auto& ps = t.particleSystem;
		const auto& transform = t.transform;

		const auto& emitter = ps->m_emitter;

		for (auto it = emitter->m_liveParticles.begin(); it != emitter->m_liveParticles.end(); )
		{
			auto* particle = *it;
			particle->m_lifetimeSeconds -= timeElapsedS;

			if (particle->m_lifetimeSeconds <= 0.0f)
			{
				it = emitter->m_liveParticles.erase(it);
				emitter->destroy(particle);
				continue;
			}

			++it;

			// Update particle parameters
			particle->m_position = Vector2Add(particle->m_position, Vector2Scale(particle->m_velocity, timeElapsedS));
			particle->m_rotation = particle->m_rotation + particle->m_angularSpeed * timeElapsedS;
			particle->m_scale = Vector2Add(particle->m_scale, Vector2Scale(particle->m_scalarSpeed, timeElapsedS));
			particle->m_alpha = particle->m_alpha + particle->m_alphaSpeed * timeElapsedS;

			//if (!ps->m_worldSpaceEmission)
			//{
			//	particle->m_position = Vector2Add(particle->m_position, transform->position());
			//}
		}

		if (!ps->m_isPlaying)
			continue;

		f32 emitParticles = ps->m_emissionRate * timeElapsedS + emitter->m_particlesToEmit;
		u32 particlesToEmit = static_cast<u32>(emitParticles);

		auto position = ps->m_worldSpaceEmission ? transform->position() : Vector2{ 0.0f, 0.0f };
		auto rotation = transform->rotation();

		for (u32 i = 0; i < particlesToEmit; ++i)
		{
			if (emitter->m_liveParticles.size() < ps->m_maxParticles)
			{
				emitter->emitParticle(position, rotation);
			}
		}

		emitter->m_particlesToEmit = emitParticles - (f32)particlesToEmit;
	}
}

auto pd::ParticleSystem::getParticleSystemTuples() -> std::vector<ParticleSystemTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("ParticleTuple"))))
		m_pEntityManager->getParticleSystemTuples(m_particleSystemTuples);
	return m_particleSystemTuples;
}
