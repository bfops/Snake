#pragma once

#include "Config.hpp"
#include "Spawn.hpp"

class Sentinel;

class Food : public Spawn
{
private:
	const Config::SpawnsData::FoodData& info;

	void SnakeCollisionHandler();

public:
	Food(const Sentinel& prototype, const Config::SpawnsData::FoodData& foodInfo);

	void CollisionHandler(WorldObject&) const;
	
	const Config::SpawnsData::FoodData& GetData() const;
};
