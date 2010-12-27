#include "Config.hpp"

#include "Logger.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <fstream>
#include <istream>

#ifdef MSVC
#pragma warning(pop)
#endif

const Config Config::gameConfig(Config::GetConfigLoader("game.cfg"));

static inline std::string get_wall_data_name(const unsigned short i, const char* const specifier)
{
	std::stringstream s;
	s << "wall"
	  << i
	  << specifier;

	return s.str();
}

const Config& Config::Get()
{
	return gameConfig;
}

const Config::ConfigScope Config::GetConfigLoader(const std::string& filename)
{
	std::ifstream configFile(filename);
	if(configFile.is_open())
		return Config::ConfigScope(configFile);

	std::stringstream defaultConfig = GetDefaultConfig();
	return Config::ConfigScope(defaultConfig);
}

Config::ColorConfig::operator Color24() const
{
	return Color24(r, g, b);
}

Config::BoundsConfig::operator Bounds() const
{
	return Bounds(min, max);
}

Config::Config(const ConfigScope in) :
	wallsConfig("walls", "wall", &in), screen(&in), spawns(&in), snake(&in), resources(&in)
{
	in.GetField("music", music);
	in.GetField("sound", sound);
	in.GetField("FPS", FPS);

	in.GetField("pointGainPeriod", pointGainPeriod);
	in.GetField("pointGainAmount", pointGainAmount);
}

Config::ConfigLoadable::ConfigLoadable(const std::string& scopeName, const ConfigScope*& in)
{
	in = in->GetScope(scopeName);
}


Config::ColorConfig::ColorConfig(const ConfigScope* in) :
	ConfigLoadable("color", in)
{
	in->GetField("r", r);
	in->GetField("g", g);
	in->GetField("b", b);
}

Config::BoundsConfig::BoundsConfig(const ConfigScope* in) :
	ConfigLoadable("bounds", in)
{
	const ConfigScope* const minScope = in->GetScope("min");
		minScope->GetField("x", min.x);
		minScope->GetField("y", min.y);
	const ConfigScope* const maxScope = in->GetScope("max");
		maxScope->GetField("x", max.x);
		maxScope->GetField("y", max.y);
}

Config::SnakeConfig::SnakeConfig(const ConfigScope* in) :
	ConfigLoadable("snake", in), color(in), head(in)
{
	in->GetField("startingLength", startingLength);
	in->GetField("width", width);
	in->GetField("startingSpeed", startingSpeed);
	in->GetField("speedupAmount", speedupAmount);
	in->GetField("speedupPeriod", speedupPeriod);
	in->GetField("growthCap", growthCap);
	in->GetField("growthRate", growthRate);
}

Config::SnakeConfig::Head::Head(const ConfigScope* in) :
	ConfigLoadable("head", in), color(in)
{
}

Config::Resources::Resources(const ConfigScope* in) :
	ConfigLoadable("resources", in)
{
	in->GetField("eat", eat);
	in->GetField("spawn", spawn);
	in->GetField("die", die);
	in->GetField("theme", theme);
}

Config::WallConfig::WallConfig(const ConfigScope* in) :
	ConfigLoadable("wall", in), bounds(in), color(in)
{
}

Config::ScreenConfig::ScreenConfig(const ConfigScope* in) :
	ConfigLoadable("screen", in), bgColor(in)
{
	in->GetField("w", w);
	in->GetField("h", h);
}

template <typename _T>
static void add_to_spawns(Config::SpawnCollectionConfig::SpawnCollection& spawns, const _T& spawn)
{
	spawns.push_back(Config::SpawnCollectionConfig::SpawnPtr(new _T(spawn)));
}

Config::SpawnCollectionConfig::SpawnCollectionConfig(const ConfigScope* in) :
	ConfigLoadable("spawns", in), bounds(in)
{
	in->GetField("period", period);
	
	// add both types of spawns to the communal spawn info list
	const LoadableCollection<FoodConfig> foods("foods", "food", in);
	const LoadableCollection<MineConfig> mines("mines", "mine", in);
	for_each(foods.begin(), foods.end(), boost::bind(&add_to_spawns<FoodConfig>, boost::ref(spawnsConfig), _1));
	for_each(mines.begin(), mines.end(), boost::bind(&add_to_spawns<MineConfig>, boost::ref(spawnsConfig), _1));
}

Config::SpawnCollectionConfig::SpawnConfig::SpawnConfig(const std::string& scopeName, const ConfigScope*& in) :
	ConfigLoadable(scopeName, in), color(in)
{
	in->GetField("size", size);
	in->GetField("cushion", cushion);
	in->GetField("expiry", expiry);
	in->GetField("rate", rate);
}

Config::SpawnCollectionConfig::FoodConfig::FoodConfig(const ConfigScope* in) :
	SpawnConfig("food", in)
{
	in->GetField("points", points);
	in->GetField("lengthFactor", lengthFactor);
	in->GetField("speedChange", speedChange);
}

Config::SpawnCollectionConfig::MineConfig::MineConfig(const ConfigScope* in) :
	SpawnConfig("mine", in)
{
}
