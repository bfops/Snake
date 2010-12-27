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

Config::ConfigScope Config::GetConfigLoader(const std::string& filename)
{
	std::ifstream configFile(filename);
	if(configFile.is_open())
		return Config::ConfigScope(configFile);

	std::stringstream defaultConfig = GetDefaultConfig();
	return Config::ConfigScope(defaultConfig);
}

Config::ColorData::operator Color24() const
{
	return Color24(r, g, b);
}

Config::BoundsData::operator Bounds() const
{
	return Bounds(min, max);
}

Config::Config(ConfigScope in) :
	wallsData("walls", "wall", &in), screen(&in), spawns(&in), snake(&in), resources(&in)
{
	in.Get("music", music);
	in.Get("sound", sound);
	in.Get("FPS", FPS);

	in.Get("pointGainPeriod", pointGainPeriod);
	in.Get("pointGainAmount", pointGainAmount);
}

Config::ConfigLoadable::ConfigLoadable(const std::string& scopeName, ConfigScope*& in)
{
	in = in->GetScope(scopeName);
}


Config::ColorData::ColorData(ConfigScope* in) :
	ConfigLoadable("color", in)
{
	in->Get("r", r);
	in->Get("g", g);
	in->Get("b", b);
}

Config::BoundsData::BoundsData(ConfigScope* in) :
	ConfigLoadable("bounds", in)
{
	ConfigScope* const minScope = in->GetScope("min");
		minScope->Get("x", min.x);
		minScope->Get("y", min.y);
	ConfigScope* const maxScope = in->GetScope("max");
		maxScope->Get("x", max.x);
		maxScope->Get("y", max.y);
}

Config::SnakeData::SnakeData(ConfigScope* in) :
	ConfigLoadable("snake", in), color(in), head(in)
{
	in->Get("startingLength", startingLength);
	in->Get("width", width);
	in->Get("startingSpeed", startingSpeed);
	in->Get("speedupAmount", speedupAmount);
	in->Get("speedupPeriod", speedupPeriod);
	in->Get("growthCap", growthCap);
	in->Get("growthRate", growthRate);
}

Config::SnakeData::Head::Head(ConfigScope* in) :
	ConfigLoadable("head", in), color(in)
{
}

Config::Resources::Resources(ConfigScope* in) :
	ConfigLoadable("resources", in)
{
	in->Get("eat", eat);
	in->Get("spawn", spawn);
	in->Get("die", die);
	in->Get("theme", theme);
}

Config::WallData::WallData(ConfigScope* in) :
	ConfigLoadable("wall", in), bounds(in), color(in)
{
}

Config::ScreenData::ScreenData(ConfigScope* in) :
	ConfigLoadable("screen", in), bgColor(in)
{
	in->Get("w", w);
	in->Get("h", h);
}

template <typename _T>
static void add_to_spawns(Config::SpawnsData::SpawnList& spawns, const _T& spawn)
{
	spawns.push_back(Config::SpawnsData::SpawnPtr(new _T(spawn)));
}

Config::SpawnsData::SpawnsData(ConfigScope* in) :
	ConfigLoadable("spawns", in), bounds(in)
{
	in->Get("period", period);
	
	// add both types of spawns to the communal spawn info list
	const LoadableList<FoodData> foods("foods", "food", in);
	const LoadableList<MineData> mines("mines", "mine", in);
	for_each(foods.begin(), foods.end(), boost::bind(&add_to_spawns<FoodData>, boost::ref(spawnsData), _1));
	for_each(mines.begin(), mines.end(), boost::bind(&add_to_spawns<MineData>, boost::ref(spawnsData), _1));
}

Config::SpawnsData::SpawnData::SpawnData(const std::string& scopeName, ConfigScope*& in) :
	ConfigLoadable(scopeName, in), color(in)
{
	in->Get("size", size);
	in->Get("cushion", cushion);
	in->Get("expiry", expiry);
	in->Get("rate", rate);
}

Config::SpawnsData::FoodData::FoodData(ConfigScope* in) :
	SpawnData("food", in)
{
	in->Get("points", points);
	in->Get("lengthFactor", lengthFactor);
	in->Get("speedChange", speedChange);
}

Config::SpawnsData::MineData::MineData(ConfigScope* in) :
	SpawnData("mine", in)
{
}
