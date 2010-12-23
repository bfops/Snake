#pragma once

#include "Bounds.hpp"
#include "Color24.hpp"

class ConfigLoader;

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
	Config(ConfigLoader);

	static const Config gameConfig;
	static std::stringstream GetDefaultConfig();

public:
	struct ColorData : public Color24
	{
	};

	struct SnakeData
	{
		unsigned long startingLength;
		unsigned short width;
		unsigned short startingSpeed;

		unsigned int speedupPeriod;
		unsigned short speedupAmount;

		double growthRate;
		unsigned long growthCap;

		ColorData color;
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

		ColorData color;
		std::vector<WallData> wallsData;
	};

	struct ScreenData
	{
		unsigned long w, h;
		ColorData bgColor;
	};

	struct SpawnsData
	{
		struct SpawnData
		{
			ColorData color;
			// square size of spawn
			unsigned short size;
			// amount of empty space around spawns
			unsigned short cushion;
			// time before spawn disappears
			unsigned int expiry;
		};

		struct FoodData : public SpawnData
		{
			long long points;
			double lengthFactor;
			// spawn rate
			double rate;
			short speedChange;
		};

		struct MineData : public SpawnData
		{
		};

		typedef std::vector<FoodData> Menu;

		unsigned int period;
		Menu foodsData;
		MineData mine;
	};

	// whether or not survival mode is on
	bool survival;
	// whether or not music/sound is on
	bool music, sound;

	unsigned short FPS;

	Bounds worldBounds;
	WallsData wallsData;
	ScreenData screen;
	SpawnsData spawns;

	unsigned int pointGainPeriod;
	long long pointGainAmount;

	SnakeData snake;
	Resources resources;

	// get the (only) configuration data
	static const Config& Get();

	static ConfigLoader GetConfigLoader(const std::string& configFileName);
};
