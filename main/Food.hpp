#pragma once

#include "Config.hpp"
#include "Spawn.hpp"

class Sentinel;

class Food : public Spawn
{
private:
	// snake growth amount is proportional to this
	double calories;
	int pointsGiven;

	void SnakeCollisionHandler();

public:
	Food(const Sentinel& prototype, unsigned int foodSize, const Config::SpawnData::FoodData& foodInfo);

	void CollisionHandler(WorldObject&) const;
	
	double GetCalories() const;
	int GetPoints() const;
};
