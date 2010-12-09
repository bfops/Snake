#include "Config.hpp"

static Config config;

Config& Config::Get()
{
	return config;
}
