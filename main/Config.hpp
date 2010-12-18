#pragma once

#include "Bounds.hpp"
#include "Color24.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <string>
#include <vector>

#ifdef MSVC
#pragma warning(pop)
#endif

// TODO: classes which load their own data, rather than this "god" function
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

		Color24 color;
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
	
	struct WallsData
	{
		struct WallData
		{
			unsigned int x, y, w, h;
		};

		Color24 color;
		std::vector<WallData> wallsData;
	};

	struct SpawnData
	{
		unsigned int period;
		// square size of spawns
		unsigned int size;
		// square size of sentinels
		unsigned int sentinelSize;
		Color24 color;
	};

	Color24 bgColor;
	// whether or not survival mode is on
	bool survival;
	// whether or not music is on
	bool music;

	unsigned short FPS;

	Bounds worldBounds;
	WallsData wallsData;
	SpawnData spawn;

	unsigned int pointGainPeriod, pointGainAmount;

	SnakeConfig snake;
	Resources resources;

	// get the (only) configuration data
	static const Config& Get();
};
