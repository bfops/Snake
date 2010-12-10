#pragma once

#include "Bounds.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <string>
#include <vector>

#ifdef MSVC
#pragma warning(pop)
#endif

struct Config
{
	struct SnakeConfig
	{
		unsigned int startingLength, width;
		unsigned int startingSpeed;

		unsigned int speedupPeriod, speedupAmount;

		double growthRate;
		unsigned int growthCap;
	};

	struct Resources
	{
		// SFX
		std::string eat;
		std::string spawn;
		std::string die;

		// musical
		std::string gameIntro, theme, startup;
	};
	
	struct WallData
	{
		unsigned int x, y, w, h;
	};

	bool survival, music;

	unsigned short FPS;

	unsigned int spawnPeriod, spawnSize;
	unsigned int sentinelSize;

	Bounds worldBounds;
	std::vector<WallData> wallData;

	unsigned int pointGainPeriod, pointGainAmount;

	SnakeConfig snake;
	Resources resources;

	Config();
	Config(std::istream& configInputStream);

	static const Config& Get();
};
