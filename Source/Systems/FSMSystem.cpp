#include <Systems/FSMSystem.h>


#include <Components/FSMData.h>


using namespace pd;


void FSMSystem::setParameter(IFiniteStateMachine& fsm, const std::string& parameter, float value)
{
	// TODO: Error?
	auto* currentState = fsm.getCurrentState();
	if (!currentState)
		return;

	// TODO: Store parameter value
	fsm.setParameterState(parameter, value);

	const auto& nextState = tryTransition(fsm , *currentState);
	if (nextState)
		fsm.setCurrentState(nextState);
}

// TODO: This is broken. We don't ever use parameter.
//  We need to store parameter values!!!
IState* FSMSystem::tryTransition(const IFiniteStateMachine& fsm, IState& state)
{
	for (const auto& t : state.getTransitions())
	{
		bool conditionMet = true;
		for (const auto& condition : t.conditions)
		{
			const auto value = fsm.getParameterState(condition.parameter);
			conditionMet &= checkCondition(condition.type, condition.threshold, value);
		}

		if (conditionMet)
			return fsm.getState(t.state);
	}

	return nullptr;
}

bool FSMSystem::checkCondition(eConditionType condition, float threshold, float value)
{
	// TODO: floating point inaccuracies
	switch (condition)
	{
	case pd::eConditionType::GreaterThan:
		return value > threshold;
	case pd::eConditionType::LessThan:
		return value < threshold;
	case pd::eConditionType::GreaterEq:
		return value >= threshold;
	case pd::eConditionType::LessEq:
		return value <= threshold;
	case pd::eConditionType::Eq:
		return value == threshold;
	case pd::eConditionType::NotEq:
		return value != threshold;
	default:
		return false;
	}
}
