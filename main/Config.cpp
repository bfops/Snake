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
	if(!stream.good())
	{
		survival = false;
		music = true;
		FPS = 60;

		worldBounds.min.x = worldBounds.min.y = 0;
		worldBounds.max.x = 800;
		worldBounds.max.y = 600;
		
		spawnPeriod = 8000;
		spawnSize = 15;
		sentinelSize = 17;

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
		ConfigLoader in(stream);

		in.CurrentScope().Get("survival", survival);
		in.CurrentScope().Get("music", music);
		in.CurrentScope().Get("FPS", FPS);

		in.CurrentScope().Get("resourceEat", resources.eat);
		in.CurrentScope().Get("resourceSpawn", resources.spawn);
		in.CurrentScope().Get("resourceDie", resources.die);
		in.CurrentScope().Get("resourceIntro", resources.gameIntro);
		in.CurrentScope().Get("resourceTheme", resources.theme);
		in.CurrentScope().Get("resourceStartup", resources.startup);
		
		while(in.EnterScope("wall"))
		{
			unsigned int x, y, w, h;
			in.CurrentScope().Get("MinX", x);
			in.CurrentScope().Get("MinY", y);
			in.CurrentScope().Get("MaxX", w);
			in.CurrentScope().Get("MaxY", h);

			w -= x;
			h -= y;

			const Config::Rectangle newWall = {x, y, w, h};
			wallData.push_back(newWall);

			in.LeaveScope();
		}
		
		in.CurrentScope().Get("worldBoundsMinX", worldBounds.min.x);
		in.CurrentScope().Get("worldBoundsMinY", worldBounds.min.y);
		in.CurrentScope().Get("worldBoundsMaxX", worldBounds.max.x);
		in.CurrentScope().Get("worldBoundsMaxY", worldBounds.max.y);

		if(survival)
		{
			in.CurrentScope().Get("mineAdditionPeriod", spawnPeriod);
			in.CurrentScope().Get("mineSize", spawnSize);
			in.CurrentScope().Get("mineSentinelSize", sentinelSize);

			in.CurrentScope().Get("survivalPointGainPeriod", pointGainPeriod);
			in.CurrentScope().Get("survivalPointGainAmount", pointGainAmount);
			in.CurrentScope().Get("survivalSnakeSpeedupPeriod", snake.speedupPeriod);
		}
		else
		{
			in.CurrentScope().Get("foodAdditionPeriod", spawnPeriod);
			in.CurrentScope().Get("foodSize", spawnSize);
			in.CurrentScope().Get("foodSentinelSize", sentinelSize);

			in.CurrentScope().Get("normalPointGainPeriod", pointGainPeriod);
			in.CurrentScope().Get("normalPointGainAmount", pointGainAmount);
			in.CurrentScope().Get("normalSnakeSpeedupPeriod", snake.speedupPeriod);
		}

		in.CurrentScope().Get("snakeDefaultLength", snake.startingLength);
		in.CurrentScope().Get("snakeWidth", snake.width);
		in.CurrentScope().Get("snakeDefaultSpeed", snake.startingSpeed);
		in.CurrentScope().Get("snakeSpeedupAmount", snake.speedupAmount);
		in.CurrentScope().Get("snakeGrowthCap", snake.growthCap);
		in.CurrentScope().Get("snakeGrowthRate", snake.growthRate);
	}
}

const Config& Config::Get()
{
	return gameConfig;
}
