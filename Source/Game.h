#pragma once

#include <Config.h>

#if (DEBUG_BUILD)
#include <chrono>
#endif

#include <Entity.h>
#include <Types.h>


namespace pd
{
	class Game
	{
	public:
		Game();
		~Game() = default;

	public:
		s32 run();

		const std::string& getBasePath() { return m_baseBath; }

	private:
		void createServices();
		void loadAssets();
		void createGameData();

	public:
		static u32 s_ScreenWidth;
		static u32 s_ScreenHeight;

#if (DEBUG_BUILD)
		static std::chrono::steady_clock::time_point s_FrameStart;
		static std::vector<std::string> s_SystemTimes;
#endif

	private:
		const std::string m_baseBath;

		EntityManager m_entityManager{};

		CommandSystem m_commandSystem;
		PlayerController m_playerController;
		CameraSystem m_cameraSystem;
		PointerAimSystem m_pointerAimSystem;
		RotateToAimSystem m_rotateToAimSystem;
		GraphicsAnimator m_graphicsAnimator;
		RenderSystem m_renderSystem;
		ParticleSystem m_particleSystem;
		PhysicsSystem m_physicsSystem;
		ProjectileSystem m_projectileSystem;
	};
}
