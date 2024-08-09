#pragma once


#include <string>
#include <vector>

#include <System.h>


namespace pd { class IFiniteStateMachine; }
namespace pd { class IState; }
namespace pd { enum class eConditionType; }


namespace pd
{
	class FSMSystem : public System
	{
	public:
		FSMSystem(observer_ptr<EntityManager> entityManager) : System(entityManager) {}
		FSMSystem() = delete;
		virtual ~FSMSystem() = default;

	public:
		void setParameterBool(IFiniteStateMachine& fsm, const std::string& parameter, bool value)
		{
			setParameter(fsm, parameter, static_cast<f32>(value));
		}

		void setParameterInt(IFiniteStateMachine& fsm, const std::string& parameter, s32 value)
		{
			setParameter(fsm, parameter, static_cast<f32>(value));
		}

		void setParameterUInt(IFiniteStateMachine& fsm, const std::string& parameter, u32 value)
		{
			setParameter(fsm, parameter, static_cast<f32>(value));
		}

		void setParameterFloat(IFiniteStateMachine& fsm, const std::string& parameter, f32 value)
		{
			setParameter(fsm, parameter, value);
		}

	private:
		void setParameter(IFiniteStateMachine& fsm, const std::string& parameter, float value);
		IState* tryTransition(const IFiniteStateMachine& fsm, IState& state);
		bool checkCondition(eConditionType type, float threshold, float value);
	};
}
