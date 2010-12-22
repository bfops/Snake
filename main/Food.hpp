#pragma once

#include "Config.hpp"
#include "Spawn.hpp"

class Sentinel;

class Food : public Spawn
{
private:
	const Config::SpawnData::FoodData& info;

	void SnakeCollisionHandler();

public:
	Food(const Sentinel& prototype, const Config::SpawnData::FoodData& foodInfo);

	void CollisionHandler(WorldObject&) const;
	
	const Config::SpawnData::FoodData& GetData() const;
};
