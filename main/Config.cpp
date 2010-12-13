#include "Config.hpp"

#include "ConfigLoader.hpp"

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

static inline std::string get_wall_data_name(const unsigned short i, const char* specifier)
{
	std::stringstream s;
	s << "wall"
	  << i
	  << specifier;

	return s.str();
}

Config::Config(std::istream& stream)
{
	ConfigLoader in(stream);

	in.Get("survival", survival);
	in.Get("music", music);
	in.Get("FPS", FPS);

	in.Get("resourceEat", resources.eat);
	in.Get("resourceSpawn", resources.spawn);
	in.Get("resourceDie", resources.die);
	in.Get("resourceIntro", resources.gameIntro);
	in.Get("resourceTheme", resources.theme);
	in.Get("resourceStartup", resources.startup);

	bool wallLoaded = true;
	for(unsigned short i = 0; wallLoaded; ++i)
	{
		unsigned int x, y, w, h;
		wallLoaded &= in.Get(get_wall_data_name(i, "MinX"), x);
		wallLoaded &= in.Get(get_wall_data_name(i, "MinY"), y);
		wallLoaded &= in.Get(get_wall_data_name(i, "MaxX"), w);
		wallLoaded &= in.Get(get_wall_data_name(i, "MaxY"), h);

		if(wallLoaded)
		{
			w -= x;
			h -= y;

			const Config::Rectangle newWall = {x, y, w, h};;
			wallData.push_back(newWall);
		}
	}

	in.Get("worldBoundsMinX", worldBounds.min.x);
	in.Get("worldBoundsMinY", worldBounds.min.y);
	in.Get("worldBoundsMaxX", worldBounds.max.x);
	in.Get("worldBoundsMaxY", worldBounds.max.y);

	if(survival)
	{
		in.Get("mineAdditionPeriod", spawnPeriod);
		in.Get("mineSize", spawnSize);
		in.Get("mineSentinelSize", sentinelSize);

		in.Get("survivalPointGainPeriod", pointGainPeriod);
		in.Get("survivalPointGainAmount", pointGainAmount);
		in.Get("survivalSnakeSpeedupPeriod", snake.speedupPeriod);
	}
	else
	{
		in.Get("foodAdditionPeriod", spawnPeriod);
		in.Get("foodSize", spawnSize);
		in.Get("foodSentinelSize", sentinelSize);

		in.Get("normalPointGainPeriod", pointGainPeriod);
		in.Get("normalPointGainAmount", pointGainAmount);
		in.Get("normalSnakeSpeedupPeriod", snake.speedupPeriod);
	}

	in.Get("snakeDefaultLength", snake.startingLength);
	in.Get("snakeWidth", snake.width);
	in.Get("snakeDefaultSpeed", snake.startingSpeed);
	in.Get("snakeSpeedupAmount", snake.speedupAmount);
	in.Get("snakeGrowthCap", snake.growthCap);
	in.Get("snakeGrowthRate", snake.growthRate);
}

const Config& Config::Get()
{
	return gameConfig;
}
