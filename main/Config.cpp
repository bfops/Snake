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

Config::Config(ConfigLoader in)
{	
	// TODO: error-checking in loading
	in.Get("survival", survival);
	in.Get("music", music);
	in.Get("sound", sound);
	in.Get("FPS", FPS);

	in.EnterScope("screen");
		in.Get("w", screen.w);
		in.Get("h", screen.h);
		in.EnterScope("color");
			in.Get("r", screen.bgColor.r);
			in.Get("g", screen.bgColor.g);
			in.Get("b", screen.bgColor.b);
		in.LeaveScope();
	in.LeaveScope();

	in.EnterScope("resources");
		in.Get("eat", resources.eat);
		in.Get("spawn", resources.spawn);
		in.Get("die", resources.die);
		in.Get("theme", resources.theme);
	in.LeaveScope();
		
	in.EnterScope("walls");
		in.EnterScope("color");
			in.Get("r", wallsData.color.r);
			in.Get("g", wallsData.color.g);
			in.Get("b", wallsData.color.b);
		in.LeaveScope();

		while(in.EnterScope("wall"))
		{
			WallsData::WallData newWall;

			in.EnterScope("min");
				in.Get("x", newWall.x);
				in.Get("y", newWall.y);
			in.LeaveScope();
			in.EnterScope("max");
				in.Get("x", newWall.w);
				in.Get("y", newWall.h);
			in.LeaveScope();

			newWall.w -= newWall.x;
			newWall.h -= newWall.y;

			wallsData.wallsData.push_back(newWall);

			in.LeaveScope();
		}
	in.LeaveScope();

	in.EnterScope("spawns");
		in.Get("period", spawns.period);
		in.EnterScope("bounds");
			in.EnterScope("min");
				in.Get("x", spawns.bounds.min.x);
				in.Get("y", spawns.bounds.min.y);
			in.LeaveScope();
			in.EnterScope("max");
				in.Get("x", spawns.bounds.max.x);
				in.Get("y", spawns.bounds.max.y);
			in.LeaveScope();
		in.LeaveScope();

		in.EnterScope("mine");
			in.Get("size", spawns.mine.size);
			in.Get("cushion", spawns.mine.cushion);
			in.Get("expiry", spawns.mine.expiry);

			in.EnterScope("color");
				in.Get("r", spawns.mine.color.r);
				in.Get("g", spawns.mine.color.g);
				in.Get("b", spawns.mine.color.b);
			in.LeaveScope();
		in.LeaveScope();
		in.EnterScope("foods");
			while(in.EnterScope("food"))
			{
				SpawnsData::FoodData food;
				
				in.Get("size", food.size);
				in.Get("cushion", food.cushion);
				in.Get("expiry", food.expiry);

				in.Get("lengthFactor", food.lengthFactor);
				in.Get("points", food.points);
				in.Get("rate", food.rate);
				in.Get("speedChange", food.speedChange);
				in.EnterScope("color");
					in.Get("r", food.color.r);
					in.Get("g", food.color.g);
					in.Get("b", food.color.b);
				in.LeaveScope();

				spawns.foodsData.push_back(food);

				in.LeaveScope();
			}
		in.LeaveScope();
	in.LeaveScope();

	in.Get("pointGainPeriod", pointGainPeriod);
	in.Get("pointGainAmount", pointGainAmount);

	in.EnterScope("snake");
		in.Get("startingLength", snake.startingLength);
		in.Get("width", snake.width);
		in.Get("startingSpeed", snake.startingSpeed);
		in.Get("speedupAmount", snake.speedupAmount);
		in.Get("speedupPeriod", snake.speedupPeriod);
		in.Get("growthCap", snake.growthCap);
		in.Get("growthRate", snake.growthRate);
		in.EnterScope("color");
			in.Get("r", snake.color.r);
			in.Get("g", snake.color.g);
			in.Get("b", snake.color.b);
		in.LeaveScope();
	in.LeaveScope();
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
