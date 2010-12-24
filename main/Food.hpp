#pragma once

#include "Config.hpp"
#include "Spawn.hpp"

class Food : public Spawn
{
public:
	Food(Point location, const Config::SpawnsData::FoodData& foodData);

	void CollisionHandler(WorldObject&) const;
};
