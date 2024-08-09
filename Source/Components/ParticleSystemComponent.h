#pragma once


#include <memory>
#include <unordered_set>

#include <AssetManager.h>
#include <Component.h>
#include <Config.h>
#include <ObjectPool.h>
#include <Service.h>


namespace pd { class ParticleSystemComponent; }


namespace pd
{
	struct ParticleArgs : public ComponentArgs
	{
		f32 lifetimeSeconds;
		Vector2 position;  // Position relative to entity transform
		f32 rotation;  // Rotation 
		Vector2 scale;
		Vector2 velocity;
		f32 angularSpeed;
		Vector2 scalarSpeed;  // TODO: instead emitter should have function getScaleFromLifetime???
		Vector3 color;
		f32 alpha;
		f32 alphaSpeed;
	};

	class Particle : public PooledObject<ObjectPool<Particle>>
	{
		using pool_type = ObjectPool<Particle>;

		friend class pool_type;
		friend class ParticleEmitter;
		friend class ParticleSystem;
		friend class RenderSystem;

	public:
		Particle() = default;
		~Particle() = default;

	protected:
		// Inherited via PooledObject
		virtual void onConstruction() override {}
		virtual void init(ObjectArgs* pArgs) override
		{
			ASSERT_MSG(pArgs, "Expected ParticleArgs was null.");

			auto args = *reinterpret_cast<ParticleArgs*>(pArgs);

			m_lifetimeSeconds = args.lifetimeSeconds;
			m_position = args.position;
			m_rotation = args.rotation;
			m_scale = args.scale;
			m_velocity = args.velocity;
			m_angularSpeed = args.angularSpeed;
			m_scalarSpeed= args.scalarSpeed;
			m_color = args.color;
			m_alpha = args.alpha;
			m_alphaSpeed = args.alphaSpeed;
		}

	public:
		const Color color() {
			u8 r = std::max(0, std::min(255, (s32)floor(m_color.x * 256.0)));
			u8 g = std::max(0, std::min(255, (s32)floor(m_color.y * 256.0)));
			u8 b = std::max(0, std::min(255, (s32)floor(m_color.z * 256.0)));
			u8 a = std::max(0, std::min(255, (s32)floor(m_alpha * 256.0)));
			return { r, g, b, a };
		}

	private:
		f32 m_lifetimeSeconds{};
		Vector2 m_position{};
		f32 m_rotation{};
		Vector2 m_scale{};
		Vector2 m_velocity{};
		f32 m_angularSpeed{};
		Vector2 m_scalarSpeed{};
		Vector3 m_color{};
		f32 m_alpha{};
		f32 m_alphaSpeed{};
	};


	class ParticleEmitter : public ObjectPool<Particle>
	{
		using base_type = ObjectPool<Particle>;

		friend class ParticleSystemComponent;
		friend class ParticleSystem;

	public:
		ParticleEmitter(ParticleSystemComponent* particleSystem, size_t size)
			: base_type(size), m_particleSystem(particleSystem) {}
		ParticleEmitter() = delete;
		~ParticleEmitter() = default;

	public:
		void emitParticle(const Vector2& position, f32 rotation);

	private:
		ParticleSystemComponent* m_particleSystem;
		f32 m_particlesToEmit{ 0.0f };
		std::unordered_set<Particle*> m_liveParticles;
	};
	

	struct ParticleSystemArgs : public ComponentArgs
	{
		std::string textureName;
		s8 drawLayer;
		f32 particleLifetime;
		f32 emissionRate;
		Vector2 initialSpeed;
		f32 initialRotation = 0.0f;
		f32 angularSpeed = 0.0f;
		Vector2 initialScale = { 1.0f, 1.0f };
		Vector2 scalarSpeed = { 0.0f, 0.0f };
		bool inheritRotation = false;
		bool autoPlay = false;
		bool worldSpaceEmission = false;
		Vector3 color = { 1.0f, 1.0f, 1.0f };
		f32 alpha = 1.0f;
		f32 alphaSpeed = 0;
		size_t maxParticleAmount = MAX_PARTICLE_POOL_SIZE;
	};

	class ParticleSystemComponent : public PooledComponent<ParticleSystemComponent>
	{
		using base_type = PooledComponent<ParticleSystemComponent>;
		using pool_type = ObjectPool<ParticleSystemComponent>;

		friend class pool_type;

		friend class ParticleEmitter;
		friend class ParticleSystem;
		friend class RenderSystem;
		friend class CommandSystem;

	public:
		ParticleSystemComponent() = default;
		~ParticleSystemComponent() = default;

	private:
		void onConstruction() override {}		  // Called when object is allocated
		void init(ObjectArgs* pArgs) override     // Called when object is created by pool
		{
			ASSERT_MSG(pArgs, "Expected ParticleSystemArgs was null.");

			auto args = *reinterpret_cast<ParticleSystemArgs*>(pArgs);

			m_emitter = std::make_unique<ParticleEmitter>(this, args.maxParticleAmount);
			m_maxParticles = args.maxParticleAmount;

			auto& assets = ServiceGetter::get<AssetManager>();
			auto* texturePtr = assets.getTexture(args.textureName);

			ASSERT_MSG(texturePtr, "Unable to get texture `%s`", args.textureName.c_str());

			m_pTexture = texturePtr;
			m_drawLayer = args.drawLayer;
			m_particleLifetimeSeconds = args.particleLifetime;
			m_worldSpaceEmission = args.worldSpaceEmission;
			m_inheritRotation = args.inheritRotation;
			m_emissionRate = args.emissionRate;
			m_initialRotation = args.initialRotation;
			m_initialSpeed = args.initialSpeed;
			m_angularSpeed = args.angularSpeed;
			m_initialScale = args.initialScale;
			m_scalarSpeed = args.scalarSpeed;
			m_particleColor = args.color;
			m_initialAlpha = args.alpha;
			m_alphaSpeed = args.alphaSpeed;
			m_isPlaying = args.autoPlay;
		}
		void play() { m_isPlaying = true; }
		void stop() { m_isPlaying = false; }

	public:
		auto getLiveParticles() const -> const std::unordered_set<Particle*>& { return m_emitter->m_liveParticles; }

	private:
		// TODO: Let's make our pool resizeable, so we can initialize it at a later point!
		//  But what will that do with our contiguous Component array?
		// Nononono... we should just finally put some object args into ObjectPool constructor!!!
		// NO... let's make a ParticleSystem component that has a unique ptr to a particle emitter
		// The particle emitter will have the ObjectPool<Particle> and will be allocated in ParticleSystems init
		//ObjectPool<Particle> m_particlePool;
		std::unique_ptr<ParticleEmitter> m_emitter{ nullptr };
		size_t m_maxParticles{};

		// TODO: EMITTER SHAPE!!!
		//   Particle collisions?

		// TODO: Should we really have a texture here?
		//   Ideally, we decouple the rendering from the particle logic?
		Texture2D* m_pTexture{ nullptr };
		
		bool m_isPlaying{};

		s8 m_drawLayer{};
		f32 m_particleLifetimeSeconds{};
		f32 m_emissionRate{};
		bool m_worldSpaceEmission{};

		bool m_inheritRotation{};

		Vector2 m_initialSpeed{};  // TODO: range between, spline
		f32 m_initialRotation{};  // TODO: range between, spline
		Vector2 m_initialScale{};  // TODO: range between, spline

		Vector3 m_particleColor{};  // TODO: Gradient; Model with what is shown in JBlows YT on ColorAnimationTool
		f32 m_initialAlpha{};  // TODO: range between, spline

		//Vector2 m_acceleration{};
		f32 m_angularSpeed{};  // TODO: range between, spline | OR amount to rotate over lifetime?
		Vector2 m_scalarSpeed{};  // TODO: range between, spline | OR amount to scale over lifetime?
		f32 m_alphaSpeed{};  // TODO: remove after gradient | OR amount over lifetime?
	};
}
