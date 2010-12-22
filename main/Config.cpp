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

static std::ifstream configFile("game.cfg");
const Config Config::gameConfig(configFile);

static inline std::string get_wall_data_name(const unsigned short i, const char* const specifier)
{
	std::stringstream s;
	s << "wall"
	  << i
	  << specifier;

	return s.str();
}

Config::Config(std::istream& stream)
{
	typedef std::auto_ptr<ConfigLoader> ConfigPtr;
	// if the stream passed isn't good, use _defaultConfig_ as our stream
	ConfigPtr in;
	if(stream.good())
		in = ConfigPtr(new ConfigLoader(stream));
	else
	{
		std::stringstream defaultConfig = GetDefaultConfig();
		in = ConfigPtr(new ConfigLoader(defaultConfig));
	}
	
	// TODO: error-checking in loading
	in->Get("survival", survival);
	in->Get("music", music);
	in->Get("sound", sound);
	in->Get("FPS", FPS);

	in->EnterScope("screen");
		in->Get("w", screen.w);
		in->Get("h", screen.h);
		in->EnterScope("color");
			in->Get("r", bgColor.r);
			in->Get("g", bgColor.g);
			in->Get("b", bgColor.b);
		in->LeaveScope();
	in->LeaveScope();

	in->EnterScope("resources");
		in->Get("eat", resources.eat);
		in->Get("spawn", resources.spawn);
		in->Get("die", resources.die);
		in->Get("theme", resources.theme);
	in->LeaveScope();
		
	in->EnterScope("walls");
		in->EnterScope("color");
			in->Get("r", wallsData.color.r);
			in->Get("g", wallsData.color.g);
			in->Get("b", wallsData.color.b);
		in->LeaveScope();

		while(in->EnterScope("wall"))
		{
			WallsData::WallData newWall;

			in->EnterScope("min");
				in->Get("x", newWall.x);
				in->Get("y", newWall.y);
			in->LeaveScope();
			in->EnterScope("max");
				in->Get("x", newWall.w);
				in->Get("y", newWall.h);
			in->LeaveScope();

			newWall.w -= newWall.x;
			newWall.h -= newWall.y;

			wallsData.wallsData.push_back(newWall);

			in->LeaveScope();
		}
	in->LeaveScope();
		
	in->EnterScope("worldBounds");
		in->EnterScope("min");
			in->Get("x", worldBounds.min.x);
			in->Get("y", worldBounds.min.y);
		in->LeaveScope();
		in->EnterScope("max");
			in->Get("x", worldBounds.max.x);
			in->Get("y", worldBounds.max.y);
		in->LeaveScope();
	in->LeaveScope();

	in->EnterScope("spawn");
		in->EnterScope("mineColor");
			in->Get("r", spawn.mineColor.r);
			in->Get("g", spawn.mineColor.g);
			in->Get("b", spawn.mineColor.b);
		in->LeaveScope();
		in->EnterScope("foods");
			while(in->EnterScope("food"))
			{
				SpawnData::FoodData food;

				in->Get("lengthFactor", food.lengthFactor);
				in->Get("points", food.points);
				in->Get("rate", food.rate);
				in->Get("speedChange", food.speedChange);
				in->EnterScope("color");
					in->Get("r", food.color.r);
					in->Get("g", food.color.g);
					in->Get("b", food.color.b);
				in->LeaveScope();

				spawn.foodsData.push_back(food);

				in->LeaveScope();
			}
		in->LeaveScope();
	in->LeaveScope();

	in->EnterScope(survival ? "survival" : "normal");
		in->Get("pointGainPeriod", pointGainPeriod);
		in->Get("pointGainAmount", pointGainAmount);

		in->EnterScope("snake");
			in->Get("speedupPeriod", snake.speedupPeriod);
		in->LeaveScope();

		in->EnterScope("spawn");
			in->Get("additionPeriod", spawn.period);
			in->Get("size", spawn.size);
			in->Get("sentinelSize", spawn.sentinelSize);
		in->LeaveScope();
	in->LeaveScope();

	in->EnterScope("snake");
		in->Get("defaultLength", snake.startingLength);
		in->Get("width", snake.width);
		in->Get("defaultSpeed", snake.startingSpeed);
		in->Get("speedupAmount", snake.speedupAmount);
		in->Get("growthCap", snake.growthCap);
		in->Get("growthRate", snake.growthRate);
		in->EnterScope("color");
			in->Get("r", snake.color.r);
			in->Get("g", snake.color.g);
			in->Get("b", snake.color.b);
		in->LeaveScope();
	in->LeaveScope();
}

const Config& Config::Get()
{
	return gameConfig;
}
