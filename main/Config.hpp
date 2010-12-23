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
	static std::stringstream GetDefaultConfig();

public:
	struct SnakeData
	{
		unsigned long startingLength;
		unsigned short width;
		unsigned short startingSpeed;

		unsigned int speedupPeriod;
		unsigned short speedupAmount;

		double growthRate;
		unsigned long growthCap;

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

	struct SpawnsData
	{
		struct SpawnData
		{
			Color24 color;
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
		// square size of spawns
		unsigned short size;
		// square size of sentinels
		unsigned short sentinelSize;
		Menu foodsData;
		MineData mine;
	};

	Color24 bgColor;
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
};
