#pragma once


#include <vector>

#include <Systems/FSMSystem.h>


namespace pd { class Graphics; }
namespace pd { class ActorCommand; }
namespace pd { class AnimationFSM; }
namespace pd { class AnimationState; }


namespace pd
{
	// TODO: I want to remove PlayerCommand; Instead have a isLocalPlayer flag in ActorCommand
	struct GraphicsAnimatorTuple
	{
		observer_ptr<AnimationFSM> animationFSM;
		observer_ptr<ActorCommand> command;
		observer_ptr<Graphics> graphics;
	};

	class GraphicsAnimator : public FSMSystem
	{
	public:
		GraphicsAnimator(observer_ptr<EntityManager> entityManager);
		GraphicsAnimator() = delete;
		~GraphicsAnimator() = default;

	public:
		void update(f32 timeElapsedS) override;
		void updateAnimationState(AnimationState& state, f32 timeElapsedS) const;

	private:
		auto getGraphicsAnimatorTuples() -> std::vector<GraphicsAnimatorTuple>&;

	private:
		std::vector<GraphicsAnimatorTuple> m_graphicsAnimatorTuples;
	};
}
