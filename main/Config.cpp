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
static const Config config(configFile);

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

	in.Pop("survival", survival);
	in.Pop("music", music);
	in.Pop("FPS", FPS);

	in.Pop("resourceEat", resources.eat);
	in.Pop("resourceSpawn", resources.spawn);
	in.Pop("resourceDie", resources.die);
	in.Pop("resourceIntro", resources.gameIntro);
	in.Pop("resourceTheme", resources.theme);
	in.Pop("resourceStartup", resources.startup);

	bool wallLoaded = true;
	for(unsigned short i = 0; wallLoaded; ++i)
	{
		unsigned int x, y, w, h;
		wallLoaded &= in.Pop(get_wall_data_name(i, "MinX"), x);
		wallLoaded &= in.Pop(get_wall_data_name(i, "MinY"), y);
		wallLoaded &= in.Pop(get_wall_data_name(i, "MaxX"), w);
		wallLoaded &= in.Pop(get_wall_data_name(i, "MaxY"), h);

		if(wallLoaded)
		{
			w -= x;
			h -= y;

			const Config::WallData newWall = {x, y, w, h};;
			wallData.push_back(newWall);
		}
	}

	in.Pop("worldBoundsMinX", worldBounds.min.x);
	in.Pop("worldBoundsMinY", worldBounds.min.y);
	in.Pop("worldBoundsMaxX", worldBounds.max.x);
	in.Pop("worldBoundsMaxY", worldBounds.max.y);

	if(survival)
	{
		in.Pop("mineAdditionPeriod", spawnPeriod);
		in.Pop("mineSize", spawnSize);
		in.Pop("mineSentinelSize", sentinelSize);

		in.Pop("survivalPointGainPeriod", pointGainPeriod);
		in.Pop("survivalPointGainAmount", pointGainAmount);
		in.Pop("survivalSnakeSpeedupPeriod", snake.speedupPeriod);
	}
	else
	{
		in.Pop("foodAdditionPeriod", spawnPeriod);
		in.Pop("foodSize", spawnSize);
		in.Pop("foodSentinelSize", sentinelSize);

		in.Pop("normalPointGainPeriod", pointGainPeriod);
		in.Pop("normalPointGainAmount", pointGainAmount);
		in.Pop("normalSnakeSpeedupPeriod", snake.speedupPeriod);
	}

	in.Pop("snakeDefaultLength", snake.startingLength);
	in.Pop("snakeWidth", snake.width);
	in.Pop("snakeDefaultSpeed", snake.startingSpeed);
	in.Pop("snakeSpeedupAmount", snake.speedupAmount);
	in.Pop("snakeGrowthCap", snake.growthCap);
	in.Pop("snakeGrowthRate", snake.growthRate);
}

const Config& Config::Get()
{
	return config;
}
