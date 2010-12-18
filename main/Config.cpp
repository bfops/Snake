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

// TODO: associate subscopes with member variables
Config::Config(std::istream& stream)
{
	if(!stream.good())
	{
		survival = false;
		music = true;
		FPS = 60;

		worldBounds.min.x = worldBounds.min.y = 0;
		worldBounds.max.x = 800;
		worldBounds.max.y = 600;
		
		spawn.period = 8000;
		spawn.size = 15;
		spawn.sentinelSize = 17;

		pointGainPeriod = 5000;
		pointGainAmount = 15;

		snake.speedupPeriod = 14000;
		snake.startingSpeed = 100;
		snake.speedupAmount = 18;

		snake.startingLength = 90;
		snake.width = 20;

		snake.growthCap = 100;
		snake.growthRate = 0.345;

		Logger::Debug("Couldn't find config! Proceeding with default config (uninitialized variables");
		return;
	}
	else
	{
		// TODO: error-checking in loading
		ConfigLoader in(stream);

		in.CurrentScope().Get("survival", survival);
		in.CurrentScope().Get("music", music);
		in.CurrentScope().Get("FPS", FPS);
		in.EnterScope("bgColor");
			in.CurrentScope().Get("r", bgColor.r);
			in.CurrentScope().Get("g", bgColor.g);
			in.CurrentScope().Get("b", bgColor.b);
		in.LeaveScope();

		in.EnterScope("resources");
			in.CurrentScope().Get("eat", resources.eat);
			in.CurrentScope().Get("spawn", resources.spawn);
			in.CurrentScope().Get("die", resources.die);
			in.CurrentScope().Get("intro", resources.gameIntro);
			in.CurrentScope().Get("theme", resources.theme);
			in.CurrentScope().Get("startup", resources.startup);
		in.LeaveScope();
		
		in.EnterScope("walls");
			in.EnterScope("color");
				in.CurrentScope().Get("r", wallsData.color.r);
				in.CurrentScope().Get("g", wallsData.color.g);
				in.CurrentScope().Get("b", wallsData.color.b);
			in.LeaveScope();

			while(in.EnterScope("wall"))
			{
				unsigned int x, y, w, h;

				in.EnterScope("min");
					in.CurrentScope().Get("x", x);
					in.CurrentScope().Get("y", y);
				in.LeaveScope();
				in.EnterScope("max");
					in.CurrentScope().Get("x", w);
					in.CurrentScope().Get("y", h);
				in.LeaveScope();

				w -= x;
				h -= y;

				const Config::WallsData::WallData newWall = {x, y, w, h};
				wallsData.wallsData.push_back(newWall);

				in.LeaveScope();
			}
		in.LeaveScope();
		
		in.EnterScope("worldBounds");
			in.EnterScope("min");
				in.CurrentScope().Get("x", worldBounds.min.x);
				in.CurrentScope().Get("y", worldBounds.min.y);
			in.LeaveScope();
			in.EnterScope("max");
				in.CurrentScope().Get("x", worldBounds.max.x);
				in.CurrentScope().Get("y", worldBounds.max.y);
			in.LeaveScope();
		in.LeaveScope();

		in.EnterScope(survival ? "survival" : "normal");
			in.CurrentScope().Get("pointGainPeriod", pointGainPeriod);
			in.CurrentScope().Get("pointGainAmount", pointGainAmount);

			in.EnterScope("snake");
				in.CurrentScope().Get("speedupPeriod", snake.speedupPeriod);
			in.LeaveScope();

			in.EnterScope("spawn");
				in.CurrentScope().Get("additionPeriod", spawn.period);
				in.CurrentScope().Get("size", spawn.size);
				in.CurrentScope().Get("sentinelSize", spawn.sentinelSize);
				if(survival)
				{
					in.EnterScope("color");
						in.CurrentScope().Get("r", spawn.color.r);
						in.CurrentScope().Get("g", spawn.color.g);
						in.CurrentScope().Get("b", spawn.color.b);
					in.LeaveScope();
				}
			in.LeaveScope();
		in.LeaveScope();

		in.EnterScope("snake");
			in.CurrentScope().Get("defaultLength", snake.startingLength);
			in.CurrentScope().Get("width", snake.width);
			in.CurrentScope().Get("defaultSpeed", snake.startingSpeed);
			in.CurrentScope().Get("speedupAmount", snake.speedupAmount);
			in.CurrentScope().Get("growthCap", snake.growthCap);
			in.CurrentScope().Get("growthRate", snake.growthRate);
			in.EnterScope("color");
				in.CurrentScope().Get("r", snake.color.r);
				in.CurrentScope().Get("g", snake.color.g);
				in.CurrentScope().Get("b", snake.color.b);
			in.LeaveScope();
		in.LeaveScope();
	}
}

const Config& Config::Get()
{
	return gameConfig;
}
