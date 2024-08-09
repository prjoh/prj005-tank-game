#pragma once


#include <Component.h>
#include <Components/FSMData.h>
#include <Raylib.h>


namespace pd
{
	// TODOs:
	//  - Expand StateData in xml (animationFrames, framesPerSecond, isLooped)
	//  - How can we split FSM mechanism in ECS way?
	//     - Split FiniteStateMachine into FSMData and FSMSystem
	//     - Split State into StateData and FSMSystem
	//     - The base FSMData/System should be able to handle the transition stuff (No extra fields for AnimationFSMData/System)
	//     - The inherited AnimationFSMSystem handles the return of current AnimtionStateData after updates have finished
	//     - AnimationStateData is then used to modify Graphics state
	//     - BUT: If state is a component, where does it live???
	//		 - StateEntity? Can one entity have multiple of same component?
	//       - Can there be non-entity components?
	// How about AnimatorState singleton? It will then store the current active state. (FSMData?)

	struct AnimationStateArgs : public ComponentArgs
	{
		std::string_view name;
		u8 animationFrames;
		f32 framesPerSecond;
		bool isLooped = false;
		std::vector<Transition> transitions{};
	};

	class AnimationState : public StateBase<AnimationState>
	{
		using animation_state_pool_type = ObjectPool<AnimationState>;
		using pooled_base_type = PooledComponent<AnimationState>;

		friend class animation_state_pool_type;

		friend class GraphicsAnimator;

	public:
		AnimationState() = default;
		~AnimationState() = default;

	public:
		void play()
		{
			m_isPlaying = true;
			m_isCompleted= false;
			m_elapsedTime = 0.0f;
			m_currentFrame = 0;
		}

		void stop()
		{
			m_isPlaying = false;
			m_isCompleted = false;
			m_elapsedTime = 0.0f;
			m_currentFrame = 0;
		}

	protected:
		void onConstruction() override {}	   // Called when object is allocated
		void init(ObjectArgs* pArgs) override  // Called when object is created by pool
		{
			ASSERT_MSG(pArgs, "Expected AnimationStateArgs was null.");

			auto args = *reinterpret_cast<AnimationStateArgs*>(pArgs);
			m_name = args.name;
			m_animationFrames = args.animationFrames;
			m_framesPerSecond = args.framesPerSecond;
			m_isLooped = args.isLooped;
			m_transitions = args.transitions;
		}

	private:
		u8 m_animationFrames{};
		f32 m_framesPerSecond{};
		bool m_isLooped{};

		f32 m_elapsedTime{ 0.0f };
		u8 m_currentFrame{ 0 };
		bool m_isPlaying{ false };
		bool m_isCompleted{ false };
	};

	struct AnimationFSMArgs : public ComponentArgs
	{
		std::string_view initialState;
	};

	class AnimationFSM : public FiniteStateMachineBase<AnimationFSM>
	{
		using animation_fsm_pool_type = ObjectPool<AnimationFSM>;
		using pooled_base_type = PooledComponent<AnimationFSM>;

		friend class animation_fsm_pool_type;

		friend class GraphicsAnimator;

	public:
		AnimationFSM() = default;
		~AnimationFSM() = default;

	protected:
		void onConstruction() override {}	   // Called when object is allocated
		void init(ObjectArgs* pArgs) override  // Called when object is created by pool
		{
			ASSERT_MSG(pArgs, "Expected AnimationFSMArgs was null.");

			auto args = *reinterpret_cast<AnimationFSMArgs*>(pArgs);
			m_initialState = args.initialState;
		}
	};
}

