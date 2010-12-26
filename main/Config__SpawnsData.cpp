#include "Config.hpp"

#include "Food.hpp"
#include "Mine.hpp"

GameWorld::SpawnPtr Config::SpawnsData::FoodData::ConstructSpawn(const Point& location) const
{
	return GameWorld::SpawnPtr(new Food(location, size + cushion, color,
		points, lengthFactor, speedChange));
}

GameWorld::SpawnPtr Config::SpawnsData::MineData::ConstructSpawn(const Point& location) const
{
	return GameWorld::SpawnPtr(new Mine(location, size + cushion, color));
}
