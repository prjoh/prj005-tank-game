#include <Components/ParticleSystemComponent.h>


#include <MathExtension.h>


void pd::ParticleEmitter::emitParticle(const Vector2& position, f32 rotation)
{
	ParticleArgs args;
	args.lifetimeSeconds = m_particleSystem->m_particleLifetimeSeconds;
	args.rotation = m_particleSystem->m_inheritRotation ? rotation : m_particleSystem->m_initialRotation;
	args.scale = m_particleSystem->m_initialScale;
	args.scalarSpeed = m_particleSystem->m_scalarSpeed;
	args.angularSpeed = m_particleSystem->m_angularSpeed;
	args.color = m_particleSystem->m_particleColor;
	args.alpha = m_particleSystem->m_initialAlpha;
	args.alphaSpeed = m_particleSystem->m_alphaSpeed;

	args.position = position;
	auto velocity = pd::vectorTransform(
		{ m_particleSystem->m_initialSpeed.x, m_particleSystem->m_initialSpeed.y, 0.0f }, 
		pd::rotation(rotation)
	);
	args.velocity = { velocity.x, velocity.y };

	m_liveParticles.emplace(create(&args));
}
