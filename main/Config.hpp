#pragma once

#include "Bounds.hpp"
#include "Color24.hpp"

class ConfigScope;

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
	Config(ConfigScope);

	static const Config gameConfig;
	static std::stringstream GetDefaultConfig();

public:
	struct ConfigLoadable
	{
		ConfigLoadable(const std::string& scopeName, ConfigScope*& in);
	};

	template <typename _T>
	struct LoadableList : public ConfigLoadable
	{
		typedef std::vector<_T> List;
		typedef typename List::iterator iterator;
		typedef typename List::const_iterator const_iterator;

		List list;

		LoadableList(const std::string& listName, const std::string& elementsName, ConfigScope* in) :
			ConfigLoadable(listName, in)
		{
			while(in->PeekScope(elementsName))
				list.push_back(_T(in));
		}
			
		iterator begin() { return list.begin(); }
		const_iterator begin() const { return list.begin(); }
		iterator end() { return list.end(); }
		const_iterator end() const { return list.end(); }
	};

	struct ColorData : public ConfigLoadable
	{
		Color24::ColorType r, g, b;

		ColorData(ConfigScope* in);

		operator Color24() const;
	};

	struct BoundsData : public ConfigLoadable
	{
		Point min, max;

		BoundsData(ConfigScope* in);

		operator Bounds() const;
	};

	struct SnakeData : public ConfigLoadable
	{
		unsigned long startingLength;
		unsigned short width;
		unsigned short startingSpeed;

		unsigned int speedupPeriod;
		unsigned short speedupAmount;

		double growthRate;
		unsigned long growthCap;

		ColorData color;

		SnakeData(ConfigScope* in);
	};

	// resource paths
	struct Resources : public ConfigLoadable
	{
		// SFX
		std::string eat;
		std::string spawn;
		std::string die;

		// musical
		std::string theme;

		Resources(ConfigScope* in);
	};
	
	struct WallData : public ConfigLoadable
	{
		BoundsData bounds;
		ColorData color;

		WallData(ConfigScope* in);
	};

	struct ScreenData : public ConfigLoadable
	{
		unsigned long w, h;
		ColorData bgColor;

		ScreenData(ConfigScope* in);
	};

	struct SpawnsData : public ConfigLoadable
	{
		struct SpawnData : public ConfigLoadable
		{
			ColorData color;
			// square size of spawn
			unsigned short size;
			// amount of empty space around spawns
			unsigned short cushion;
			// time before spawn disappears
			unsigned int expiry;
			// spawn rate
			double rate;

			SpawnData(const std::string& spawnScope, ConfigScope*& in);
		};

		struct FoodData : public SpawnData
		{
			long long points;
			double lengthFactor;
			short speedChange;

			FoodData(ConfigScope* in);
		};

		struct MineData : public SpawnData
		{
			MineData(ConfigScope* in);
		};

		typedef LoadableList<FoodData> Menu;
		typedef LoadableList<MineData> MineList;
		
		// spawn bounds
		BoundsData bounds;
		unsigned int period;
		Menu foodsData;
		MineList minesData;

		SpawnsData(ConfigScope* in);
	};

	// whether or not survival mode is on
	bool survival;
	// whether or not music/sound is on
	bool music, sound;

	unsigned short FPS;

	LoadableList<WallData> wallsData;
	ScreenData screen;
	SpawnsData spawns;

	unsigned int pointGainPeriod;
	long long pointGainAmount;

	SnakeData snake;
	Resources resources;

	// get the (only) configuration data
	static const Config& Get();

	static ConfigScope GetConfigLoader(const std::string& configFileName);
};
