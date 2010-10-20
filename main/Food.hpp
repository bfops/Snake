#pragma once

#include "WorldObject.hpp"

class SentinelFood;

class Food : public WorldObject
{
public:
	struct FoodInfo
	{
		const double calories;
		const Color24 color;
		const int points;

		FoodInfo(double calories, Color24 color, int pointsGiven);
	};

private:
	bool eaten;
	// growth amount is proportional to this
	double calories;
	int pointsGiven;

	void SnakeCollisionHandler();

public:
	static const FoodInfo ice, celery, normal, donut;

	Food(const SentinelFood& prototype, const FoodInfo& foodInfo);

	void CollisionHandler(const WorldObject&);

	bool IsEaten() const;
	double GetCalories() const;
	int GetPoints() const;
};
