#include <Assets.h>


namespace pd
{
	const std::vector<std::string> getTexturePaths()
	{
		return {
			"Assets/Textures/ps3.png",
			"Assets/Textures/xbox.png",
			"Assets/Textures/sh_player.png",
			"Assets/Textures/sh_tiles_01.png",
			"Assets/Textures/particle.png",
			"Assets/Textures/tire_track.png",
			"Assets/Textures/light_shell.png",
		};
	}

	const std::string getGameDataPath()
	{
		return "Assets/Data/entities.xml";
	}

	const std::vector<std::string> getLevelDataPaths()
	{
		return {
			"Assets/Data/Levels/test",
		};
	}
}
