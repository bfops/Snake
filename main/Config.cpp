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

Config::ColorData::operator Color24() const
{
	return Color24(r, g, b);
}

Config::BoundsData::operator Bounds() const
{
	return Bounds(min, max);
}

Config::Config(const ConfigScope in) :
	wallsData("walls", "wall", &in), screen(&in), spawns(&in), snake(&in), resources(&in)
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


Config::ColorData::ColorData(const ConfigScope* in) :
	ConfigLoadable("color", in)
{
	in->GetField("r", r);
	in->GetField("g", g);
	in->GetField("b", b);
}

Config::BoundsData::BoundsData(const ConfigScope* in) :
	ConfigLoadable("bounds", in)
{
	const ConfigScope* const minScope = in->GetScope("min");
		minScope->GetField("x", min.x);
		minScope->GetField("y", min.y);
	const ConfigScope* const maxScope = in->GetScope("max");
		maxScope->GetField("x", max.x);
		maxScope->GetField("y", max.y);
}

Config::SnakeData::SnakeData(const ConfigScope* in) :
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

Config::SnakeData::Head::Head(const ConfigScope* in) :
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

Config::WallData::WallData(const ConfigScope* in) :
	ConfigLoadable("wall", in), bounds(in), color(in)
{
}

Config::ScreenData::ScreenData(const ConfigScope* in) :
	ConfigLoadable("screen", in), bgColor(in)
{
	in->GetField("w", w);
	in->GetField("h", h);
}

template <typename _T>
static void add_to_spawns(Config::SpawnsData::SpawnList& spawns, const _T& spawn)
{
	spawns.push_back(Config::SpawnsData::SpawnPtr(new _T(spawn)));
}

Config::SpawnsData::SpawnsData(const ConfigScope* in) :
	ConfigLoadable("spawns", in), bounds(in)
{
	in->GetField("period", period);
	
	// add both types of spawns to the communal spawn info list
	const LoadableList<FoodData> foods("foods", "food", in);
	const LoadableList<MineData> mines("mines", "mine", in);
	for_each(foods.begin(), foods.end(), boost::bind(&add_to_spawns<FoodData>, boost::ref(spawnsData), _1));
	for_each(mines.begin(), mines.end(), boost::bind(&add_to_spawns<MineData>, boost::ref(spawnsData), _1));
}

Config::SpawnsData::SpawnData::SpawnData(const std::string& scopeName, const ConfigScope*& in) :
	ConfigLoadable(scopeName, in), color(in)
{
	in->GetField("size", size);
	in->GetField("cushion", cushion);
	in->GetField("expiry", expiry);
	in->GetField("rate", rate);
}

Config::SpawnsData::FoodData::FoodData(const ConfigScope* in) :
	SpawnData("food", in)
{
	in->GetField("points", points);
	in->GetField("lengthFactor", lengthFactor);
	in->GetField("speedChange", speedChange);
}

Config::SpawnsData::MineData::MineData(const ConfigScope* in) :
	SpawnData("mine", in)
{
}
