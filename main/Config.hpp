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

// game configuration
struct Config
{
private:
	Config(std::istream& configInputStream);

	static const Config gameConfig;

public:
	struct SnakeConfig
	{
		unsigned int startingLength, width;
		unsigned int startingSpeed;

		unsigned int speedupPeriod, speedupAmount;

		double growthRate;
		unsigned int growthCap;
	};

	// resource paths
	struct Resources
	{
		// SFX
		std::string eat;
		std::string spawn;
		std::string die;

		// musical
		std::string gameIntro;
		std::string theme;
		std::string startup;
	};
	
	struct Rectangle
	{
		unsigned int x, y, w, h;
	};

	// whether or not survival mode is on
	bool survival;
	// whether or not music is on
	bool music;

	unsigned short FPS;

	unsigned int spawnPeriod;
	// square size of spawns
	unsigned int spawnSize;
	// square size of sentinels
	unsigned int sentinelSize;

	Bounds worldBounds;
	std::vector<Rectangle> wallData;

	unsigned int pointGainPeriod, pointGainAmount;

	SnakeConfig snake;
	Resources resources;

	// get the (only) configuration data
	static const Config& Get();
};
