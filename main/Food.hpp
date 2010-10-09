#pragma once

#include "WorldObject.hpp"

class SentinelFood;

class Food : public WorldObject
{
public:
	struct FoodInfo
	{
		double calories;
		Color24 color;

		FoodInfo(double calories, Color24 color);
	};

private:
	bool eaten;
	// growth amount is proportional to this
	double calories;

	void SnakeCollisionHandler();

public:
	static const FoodInfo ice, celery, normal, donut;

	Food(SentinelFood& prototype, const FoodInfo& foodInfo);

	void CollisionHandler(const WorldObject&);

	bool IsEaten() const;
	double GetCalories() const;
};
