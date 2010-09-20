#pragma once

#include "Common.hpp"
#include "Point.hpp"
#include "SentinelFood.hpp"
#include "WorldObject.hpp"

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
	double calories;

	void SnakeCollisionHandler();

public:
	static const FoodInfo ice, celery, normal, donut;

	Food(SentinelFood& prototype, const FoodInfo& foodInfo);

	void CollisionHandler(const WorldObject&);

	bool IsEaten() const;
	double GetCalories() const;
};
