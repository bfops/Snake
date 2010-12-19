#pragma once

#include "Bounds.hpp"
#include "Color24.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <sstream>
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
	static std::stringstream defaultConfig;

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
		std::string theme;
	};
	
	struct WallsData
	{
		struct WallData
		{
			unsigned long x, y, w, h;
		};

		Color24 color;
		std::vector<WallData> wallsData;
	};

	struct ScreenData
	{
		unsigned long w, h;
		Color24 bgColor;
	};

	struct SpawnData
	{
		struct FoodData
		{
			Color24 color;
			short points;
			double lengthFactor;
			// spawn rate
			double rate;
		};
		typedef std::vector<FoodData> Menu;

		unsigned int period;
		// square size of spawns
		unsigned int size;
		// square size of sentinels
		unsigned int sentinelSize;
		Color24 mineColor;
		Menu foodsData;
	};

	Color24 bgColor;
	// whether or not survival mode is on
	bool survival;
	// whether or not music is on
	bool music;

	unsigned short FPS;

	Bounds worldBounds;
	WallsData wallsData;
	ScreenData screen;
	SpawnData spawn;

	unsigned int pointGainPeriod, pointGainAmount;

	SnakeConfig snake;
	Resources resources;

	// get the (only) configuration data
	static const Config& Get();
};
