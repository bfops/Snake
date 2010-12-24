#include "Config.hpp"

#include "ConfigLoader.hpp"
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

ConfigLoader Config::GetConfigLoader(const std::string& filename)
{
	std::ifstream configFile(filename);
	if(configFile.is_open())
		return ConfigLoader(configFile);

	std::stringstream defaultConfig = GetDefaultConfig();
	return ConfigLoader(defaultConfig);
}

Config::ColorData::operator Color24() const
{
	return Color24(r, g, b);
}

Config::BoundsData::operator Bounds() const
{
	return Bounds(min, max);
}

Config::Config(ConfigLoader in) :
	wallsData("walls", "wall", in), screen(in), spawns(in), snake(in), resources(in)
{
	in.Get("survival", survival);
	in.Get("music", music);
	in.Get("sound", sound);
	in.Get("FPS", FPS);

	in.Get("pointGainPeriod", pointGainPeriod);
	in.Get("pointGainAmount", pointGainAmount);
}

Config::ConfigLoadable::ConfigLoadable(const std::string& scopeName, ConfigLoader& in)
{
	in.EnterScope(scopeName);
}

Config::ColorData::ColorData(ConfigLoader& in) :
	ConfigLoadable("color", in)
{
		in.Get("r", r);
		in.Get("g", g);
		in.Get("b", b);
	in.LeaveScope();
}

Config::BoundsData::BoundsData(ConfigLoader& in) :
	ConfigLoadable("bounds", in)
{
		in.EnterScope("min");
			in.Get("x", min.x);
			in.Get("y", min.y);
		in.LeaveScope();
		in.EnterScope("max");
			in.Get("x", max.x);
			in.Get("y", max.y);
		in.LeaveScope();
	in.LeaveScope();
}

Config::SnakeData::SnakeData(ConfigLoader& in) :
	ConfigLoadable("snake", in), color(in)
{
		in.Get("startingLength", startingLength);
		in.Get("width", width);
		in.Get("startingSpeed", startingSpeed);
		in.Get("speedupAmount", speedupAmount);
		in.Get("speedupPeriod", speedupPeriod);
		in.Get("growthCap", growthCap);
		in.Get("growthRate", growthRate);
	in.LeaveScope();
}

Config::Resources::Resources(ConfigLoader& in) :
	ConfigLoadable("resources", in)
{
		in.Get("eat", eat);
		in.Get("spawn", spawn);
		in.Get("die", die);
		in.Get("theme", theme);
	in.LeaveScope();
}

Config::WallData::WallData(ConfigLoader& in) :
	ConfigLoadable("wall", in), bounds(in), color(in)
{
	in.LeaveScope();
}

Config::ScreenData::ScreenData(ConfigLoader& in) :
	ConfigLoadable("screen", in), bgColor(in)
{
		in.Get("w", w);
		in.Get("h", h);
	in.LeaveScope();
}

Config::SpawnsData::SpawnsData(ConfigLoader& in) :
	ConfigLoadable("spawns", in), mine(in), bounds(in), foodsData("foods", "food", in)
{
		in.Get("period", period);
	in.LeaveScope();
}

Config::SpawnsData::SpawnData::SpawnData(const std::string& scopeName, ConfigLoader& in) :
	ConfigLoadable(scopeName, in), color(in)
{
	in.Get("size", size);
	in.Get("cushion", cushion);
	in.Get("expiry", expiry);
}

Config::SpawnsData::FoodData::FoodData(ConfigLoader& in) :
	SpawnData("food", in)
{
		in.Get("points", points);
		in.Get("lengthFactor", lengthFactor);
		in.Get("rate", rate);
		in.Get("speedChange", speedChange);
	in.LeaveScope();
}

Config::SpawnsData::MineData::MineData(ConfigLoader& in) :
	SpawnData("mine", in)
{
	in.LeaveScope();
}
