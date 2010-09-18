#pragma once

#include "Common.hpp"
#include "Point.hpp"
#include "SentinelFood.hpp"
#include "WorldObject.hpp"

// TODO: put food in gameworld
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
	static DEF_CONSTANT(FoodInfo, ice, FoodInfo(-2.5, Color24(0, 0, 255)))
	static DEF_CONSTANT(FoodInfo, celery, FoodInfo(0.3, Color24(127, 255, 127)))
	static DEF_CONSTANT(FoodInfo, normal, FoodInfo(1.0, Color24(0, 255, 255)))
	static DEF_CONSTANT(FoodInfo, donut, FoodInfo(3.0, Color24(200, 0, 0)))

	Food(SentinelFood& prototype, const FoodInfo& foodInfo);

	void CollisionHandler(const WorldObject&);

	bool IsEaten() const;
	double GetCalories() const;
};
