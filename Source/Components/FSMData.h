#pragma once


#include <string>
#include <unordered_map>

#include <Component.h>
#include <Error.h>
#include <Types.h>


namespace pd
{
	enum class eConditionType
	{
		GreaterThan,
		LessThan,
		GreaterEq,
		LessEq,
		Eq,
		NotEq,
	};
	extern const std::unordered_map<size_t, eConditionType> g_ConditionTypes;

	struct Condition
	{
		std::string parameter;
		eConditionType type;
		float threshold;
	};

	struct Transition
	{
		std::string state;
		std::vector<Condition> conditions;
	};

	class IState
	{
	public:
		virtual ~IState() = default;
	public:
		virtual const std::string& getName() const = 0;
		virtual std::vector<Transition>& getTransitions() = 0;
	};

	template <class T>
	class StateBase : public PooledComponent<T>, public IState
	{
		friend class FSMSystem;

	public:
		StateBase() = default;
		virtual ~StateBase() = default;

	public:
		//virtual void onEnter() {}
		//virtual void onUpdate() {}
		//virtual void onExit() {}

		const std::string& getName() const override { return m_name; }
		std::vector<Transition>& getTransitions() override { return m_transitions; }
		//const std::string_view tryTransition(const std::string& parameter, float value)
		//{
		//	auto it = m_transitions.find(std::hash<std::string_view>{}(parameter));

		//	// TODO: Error?
		//	if (it == m_transitions.end())
		//		return {};

		//	const auto& transition = it->second;
		//	if (checkCondition(transition.condition, transition.threshold, value))
		//		return transition.state;

		//	return {};
		//}

	//	bool checkCondition(eTransitionCondition condition, float threshold, float value)
	//	{
	//		// TODO: floating point inaccuracies
	//		switch (condition)
	//		{
	//		case pd::eTransitionCondition::GreaterThan:
	//			return value > threshold;
	//		case pd::eTransitionCondition::LessThan:
	//			return value < threshold;
	//		case pd::eTransitionCondition::GreaterEq:
	//			return value >= threshold;
	//		case pd::eTransitionCondition::LessEq:
	//			return value <= threshold;
	//		case pd::eTransitionCondition::Eq:
	//			return value == threshold;
	//		case pd::eTransitionCondition::NotEq:
	//			return value != threshold;
	//		default:
	//			return false;
	//		}
	//	}

	protected:
		std::string m_name{};
		std::vector<Transition> m_transitions{};
	};

	
	class IFiniteStateMachine
	{
	public:
		virtual ~IFiniteStateMachine() = default;
	public:
		virtual void addState(IState* state) = 0;
		virtual IState* getState(const std::string_view& name) const = 0;
		virtual void setCurrentState(const IState* state) = 0;
		virtual IState* getCurrentState() = 0;
		virtual void setParameterState(const std::string& parameter, f32 value) = 0;
		virtual f32 getParameterState(const std::string& parameter) const = 0;
	};


	template <class T>
	class FiniteStateMachineBase : public PooledComponent<T>, public IFiniteStateMachine
	{
		using fsm_pool_type = ObjectPool<T>;
		using pooled_base_type = PooledComponent<T>;

		friend T;
		friend class FSMSystem;

	public:
		FiniteStateMachineBase() = default;
		virtual ~FiniteStateMachineBase() = default;

	public:
		void addState(IState* state) override {
			const auto& key = std::hash<std::string_view>{}(state->getName());
			ASSERT_MSG(m_states.find(key) == m_states.end(), "Tried adding a duplicate state.");
			m_states[key] = state;

			// Update parameter state map
			for (const auto& t : state->getTransitions())
			{
				for (const auto& c : t.conditions)
				{
					const auto& key = std::hash<std::string_view>{}(c.parameter);
					auto it = m_parameterStates.find(key);
					if (it == m_parameterStates.end())
						m_parameterStates.emplace(std::make_pair(key, 0.0f));  // TODO: We should probably allow initial parameter value
				}
			}
		}

		IState* getState(const std::string_view& name) const override {
			auto it = m_states.find(std::hash<std::string_view>{}(name));
			if (it == m_states.end())
				return nullptr;
			return it->second;
		}

		//void addStateTransition(const std::string_view& fromState, const std::string_view& toState, const std::string_view& parameter, eTransitionCondition condition, float threshold)
		//{
		//	auto it = m_states.find(std::hash<std::string_view>{}(fromState));
		//	// TODO: Error!
		//	if (it == m_states.end())
		//		return;

		//	it->second.addTransition(toState, parameter, condition, threshold);
		//}

		// TODO: Error?
		void setCurrentState(const IState* state) override {
			auto iter = m_states.find(std::hash<std::string_view>{}(state->getName()));
			if (iter != m_states.end()) {
				m_currentState = iter->second;
			}
		}

		IState* getCurrentState() override
		{
			if (!m_currentState)
			{
				m_currentState = getState(m_initialState);
				ASSERT_MSG(m_currentState, "Unable to initialize initial state. State not found.");
			}
			return m_currentState;
		}

		void setParameterState(const std::string& parameter, f32 value) override
		{
			auto key = std::hash<std::string_view>{}(parameter);
			ASSERT_MSG(m_parameterStates.find(key) != m_parameterStates.end(),
					   "No parameter `%s` found in FSM.", parameter.c_str());
			m_parameterStates[key] = value;
		}

		f32 getParameterState(const std::string& parameter) const override
		{
			auto key = std::hash<std::string_view>{}(parameter);
			ASSERT_MSG(m_parameterStates.find(key) != m_parameterStates.end(),
						"No parameter `%s` found in FSM.", parameter.c_str());
			return m_parameterStates.at(key);
		}

		//void setParameterBool(const std::string& parameter, bool value)
		//{
		//	setParameter(parameter, static_cast<f32>(value));
		//}

		//void setParameterInt(const std::string& parameter, s32 value)
		//{
		//	setParameter(parameter, static_cast<f32>(value));
		//}

		//void setParameterUInt(const std::string& parameter, u32 value)
		//{
		//	setParameter(parameter, static_cast<f32>(value));
		//}

		//void setParameterFloat(const std::string& parameter, f32 value)
		//{
		//	setParameter(parameter, value);
		//}

	//protected:
	//	void setParameter(const std::string& parameter, float value)
	//	{
	//		// TODO: Error?
	//		if (!m_currentState)
	//			return;

	//		const auto& nextState = m_currentState->tryTransition(parameter, value);
	//		if (nextState.size() > 0)
	//			setCurrentState(nextState);
	//	}

	protected:
		std::string_view m_initialState{};
		std::unordered_map<size_t, IState*> m_states{};
		IState* m_currentState{ nullptr };
		std::unordered_map<size_t, f32> m_parameterStates{};
	};
}
