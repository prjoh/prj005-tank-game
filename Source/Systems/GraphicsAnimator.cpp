#include <Systems/GraphicsAnimator.h>


#include <Components/ActorCommand.h>
#include <Components/Graphics.h>
#include <Entity.h>
#include <Utils.h>


pd::GraphicsAnimator::GraphicsAnimator(observer_ptr<EntityManager> entityManager)
	: FSMSystem(entityManager)
{
	m_componentGroups = {
		{utils::stringHash("AnimatorTuple"), {
			typeid(AnimationFSM),
			typeid(ActorCommand),
			typeid(Graphics),
		}},
	};
}

void pd::GraphicsAnimator::update(f32 timeElapsedS)
{
	for (const auto& t : getGraphicsAnimatorTuples())
	{
		auto& animationFSM = *t.animationFSM;
		const auto& command = *t.command;

		// Update current state
		auto* state = animationFSM.getCurrentState();
		auto* animState = dynamic_cast<AnimationState*>(state);
		updateAnimationState(*animState, timeElapsedS);

		// Check transitions
		const auto& movement = command.moveXY();
		setParameterFloat(animationFSM, "moveX", movement.x);
		setParameterFloat(animationFSM, "moveY", movement.y);
		auto* newState = animationFSM.getCurrentState();
		if (newState != state)
		{
			animState->stop();
			animState = dynamic_cast<AnimationState*>(newState);
			animState->play();
		}

		// Update graphics animation parameters
		auto& graphics = *t.graphics;
		graphics.m_currentFrame = animState->m_currentFrame;

		const std::string stateText = "State: " + animState->getName();
		DrawText(stateText.c_str(), 20, 20, 20, WHITE);
	}
}

void pd::GraphicsAnimator::updateAnimationState(AnimationState& state, f32 timeElapsedS) const
{
	if (!state.m_isPlaying)
		return;

	state.m_elapsedTime += timeElapsedS;

	// Update m_currentFrame
	if (state.m_elapsedTime >= 1.0f / state.m_framesPerSecond)
	{
		state.m_currentFrame = (state.m_currentFrame + 1) % state.m_animationFrames;
		state.m_elapsedTime = 0.0f;
	}


	// Check if we are finished
	if (state.m_currentFrame >= state.m_animationFrames)
	{
		if (state.m_isLooped)
		{
			// TODO
		}
		else
		{
			state.m_isPlaying = false;
			state.m_isCompleted = true;
		}
	}
}

auto pd::GraphicsAnimator::getGraphicsAnimatorTuples() -> std::vector<GraphicsAnimatorTuple>&
{
	if (m_pEntityManager->hasSystemTupleUpdates(m_componentGroups.at(utils::stringHash("AnimatorTuple"))))
		m_pEntityManager->getGraphicsAnimatorTuples(m_graphicsAnimatorTuples);
	return m_graphicsAnimatorTuples;
}

// TODO: We need to define a specific Animation Logic for every animated Graphics component
//    What are our options?
//     - Give each Graphics an identifier that we can query in updateGraphicsAnimationFSM
//     - We create a folder called `AnimationStates` that contain final classes of AnimationState for each Entity (e.g. AnimationStates/Player/TracksLeftDriveState)
//       Graphics component has a new field `States` which indicate what states to initialize in GraphicsAnimationFSM when loading data

// GraphicsAnimator: 
//   GraphicsAnimation
//    - animationFrames
//    - framesPerSecond
//    - elapsedTime
//	  - currentFrame
//    - isLooped
//    - isPlaying
//    - isCompleted

// Let user create States
// Let user create StateTransition(State, State, Condition)
// Rules:
//  LessThan
//  GreaterThan
//  LessEq
//  GreaterEq
//  Eq
//  NotEq
//  True
//  False
//  OR
//  AND
// Each frame check which conditions are met
// E.g. LeftTrackIdle, LeftTrackDrive, GreaterThan("moveX", 0.0f)
//      RightTrackIdle, RightTrackDrive, LessThan("moveX", 0.0f)

// UpdateAnimationFSM: setParameter("moveX", move.x), checkTranstions()
// GraphicsAnimator: AnimationFSM.getState(), GraphicsAnimation

//void pd::GraphicsAnimator::updateAnimationFSM(const ActorCommand& command, AnimationFSM& animationFSM, f32 elapsedTimeS) const
//{
//
//
//	//if (movement.x < 0.0f)
//	//{
//	//	// Left tracks move backward
//	//	// Right tracks move forward
//	//}
//	//else if (movement.x > 0.0f)
//	//{
//	//	// Left tracks move forward
//	//	// Right tracks move backward
//	//}
//}
