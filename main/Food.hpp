#pragma once

#include "Config.hpp"
#include "Spawn.hpp"

class Sentinel;

class Food : public Spawn
{
private:
	// snake growth amount is proportional to this
	double calories;
	long pointsGiven;

	void SnakeCollisionHandler();

public:
	Food(const Sentinel& prototype, const Config::SpawnData::FoodData& foodInfo);

	void CollisionHandler(WorldObject&) const;
	
	double GetCalories() const;
	long long GetPoints() const;
};
