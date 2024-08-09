//#pragma once
//
//
//#include <Components/ActorCommand.h>
//
//
//namespace pd
//{
//	struct PlayerCommandArgs : public ComponentArgs
//	{
//		bool isLocalPlayer;
//	};
//
//
//	class PlayerCommand : public ActorCommandBase<PlayerCommand>
//	{
//		using command_pool_type = ObjectPool<PlayerCommand>;
//		using pooled_base_type = PooledComponent<PlayerCommand>;
//
//		friend class command_pool_type;
//
//		friend class CommandSystem;
//		friend class PlayerController;
//
//	public:
//		PlayerCommand() = default;
//		~PlayerCommand() = default;
//
//	protected:
//		//void onConstruction() override {}		   // Called when object is allocated
//		void init(ObjectArgs* pArgs) override  // Called when object is created by pool
//		{
//			ASSERT_MSG(pArgs, "Expected PlayerCommandArgs was null.");
//
//			auto args = *reinterpret_cast<PlayerCommandArgs*>(pArgs);
//			m_isLocalPlayer = args.isLocalPlayer;
//		}
//
//	public:
//		//void destroy() override {}
//		//void reset() override {}
//
//	private:
//		bool m_isLocalPlayer{};
//	};
//}