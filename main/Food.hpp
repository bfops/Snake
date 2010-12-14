#pragma once

#include "WorldObject.hpp"

class Sentinel;

class Food : public WorldObject
{
public:
	struct FoodInfo
	{
	public:
		// proportional to how much the snake grows when eating
		double calories;
		Color24 color;
		int pointsGiven;

		FoodInfo(double calories, Color24 color, int pointsGiven);
	};

private:
	// snake growth amount is proportional to this
	double calories;
	int pointsGiven;

	void SnakeCollisionHandler();

public:
	static const FoodInfo ice, celery, normal, donut;

	Food(const Sentinel& prototype, unsigned int foodSize, const FoodInfo& foodInfo);

	void CollisionHandler(WorldObject&) const;
	
	double GetCalories() const;
	int GetPoints() const;
};
