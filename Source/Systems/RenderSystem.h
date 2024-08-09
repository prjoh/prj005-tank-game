#pragma once


#include <vector>

#include <System.h>


namespace pd { class Camera; }
namespace pd { class Graphics; }
namespace pd { class Transform; }
namespace pd { class ParticleSystemComponent; }
namespace pd { class DynamicPhysicsComponent; }
namespace pd { class StaticPhysicsComponent; }
namespace pd { class KinematicPhysicsComponent; }
namespace pd { struct DynamicPhysicsTuple; }
namespace pd { struct StaticPhysicsTuple; }
namespace pd { struct KinematicPhysicsTuple; }

namespace pd
{
	struct EntityRenderTuple
	{
		observer_ptr<Transform> transform;
		observer_ptr<Graphics> graphics;
	};

	struct ParticlesRenderTuple
	{
		observer_ptr<ParticleSystemComponent> particleSystem;
		observer_ptr<Transform> transform;
	};

	class RenderSystem : public System
	{
	public:
		RenderSystem(observer_ptr<EntityManager> entityManager);
		RenderSystem() = delete;
		~RenderSystem() = default;

	public:
		void update(f32 timeElapsedS) override;

	private:
		auto getEntityRenderTuples() -> std::vector<EntityRenderTuple>&;
		auto getParticlesRenderTuples() -> std::vector<ParticlesRenderTuple>&;
		auto getDynPhysicsDebugRenderTuples() -> std::vector<DynamicPhysicsTuple>&;
		auto getStaPhysicsDebugRenderTuples() -> std::vector<StaticPhysicsTuple>&;
		auto getKinPhysicsDebugRenderTuples() -> std::vector<KinematicPhysicsTuple>&;

		void render(
			const Camera& camera, 
			const std::vector<EntityRenderTuple>& entityRenderTuples,
			const std::vector<ParticlesRenderTuple>& particleRenderTuples) const;
#if (DEBUG_BUILD)
		void debugRender(const Camera& camera);

		void drawStats(u32 liveParticles) const;
#endif

	private:
		std::vector<EntityRenderTuple> m_entityRenderTuples;
		std::vector<ParticlesRenderTuple> m_particleRenderTuples;

#if (DEBUG_BUILD)
		std::vector<DynamicPhysicsTuple> m_dynPhysicsTuples;
		std::vector<StaticPhysicsTuple> m_staPhysicsTuples;
		std::vector<KinematicPhysicsTuple> m_kinPhysicsTuples;
#endif
	};
}
