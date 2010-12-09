#pragma once

#include "Bounds.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <string>

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

	bool survival, music;

	unsigned short FPS;

	unsigned int spawnPeriod, spawnSize;
	unsigned int sentinelSize;

	unsigned int wallThickness;
	Bounds worldBounds;

	unsigned int pointGainPeriod, pointGainAmount;

	SnakeConfig snake;
	Resources resources;

	static Config& Get();
};
