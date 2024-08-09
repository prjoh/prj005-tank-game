#pragma once


#include <raylib.h>

#include <Components/Command.h>


namespace pd
{
	struct ActorCommandArgs : public ComponentArgs
	{
		bool isPlayer;
		bool observer = false;
	};

	//template <class T>
	//class ActorCommandBase : public CommandComponent<T>
	class ActorCommand : public CommandComponent<ActorCommand>
	{
		//using base_type = ActorCommandBase<T>;
		//using command_pool_type = ObjectPool<T>;
		//using pooled_base_type = PooledComponent<T>;

		using actor_command_pool_type = ObjectPool<ActorCommand>;
		using pooled_base_type = PooledComponent<ActorCommand>;

		friend class actor_command_pool_type;

		friend class CommandSystem;
		friend class PlayerController;
		friend class GraphicsAnimator;
		friend class PointerAimSystem;
		friend class RotateToAimSystem;
		friend class ProjectileSystem;

	public:
		ActorCommand() { reset(); };
		virtual ~ActorCommand() = default;
		//ActorCommandBase() { reset(); };
		//virtual ~ActorCommandBase() = default;

	protected:
		void onConstruction() override {}		// Called when object is allocated
		void init(ObjectArgs* pArgs) override	// Called when object is created by pool
		{
			ASSERT_MSG(pArgs, "Expected ActorCommandArgs was null.");
	
			auto args = *reinterpret_cast<ActorCommandArgs*>(pArgs);
			m_isPlayer = args.isPlayer;
			m_observer = args.observer;
		} 

	public:
		//void destroy() override {}

		void reset() override
		{
			m_moveXY = { 0.0f, 0.0f };
			m_shoot = false;
		}

		const Vector2& moveXY() const { return m_moveXY; }
		bool shoot() const { return m_shoot; }

	protected:
		bool m_isPlayer;
		bool m_observer;  // Used as filter in Controller

		Vector2 m_moveXY{};
		Vector2 m_aimAt{};
		bool m_shoot{};
	};

	//class ActorCommand final : public ActorCommandBase<ActorCommand> {};
}
