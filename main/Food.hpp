#pragma once

#include "WorldObject.hpp"

class Sentinel;

class Food : public WorldObject
{
public:
	struct FoodInfo
	{
	private:
		FoodInfo& operator=(const FoodInfo&);

	public:
		const double calories;
		const Color24 color;
		const int points;

		FoodInfo(double calories, Color24 color, int pointsGiven);
	};

private:
	// growth amount is proportional to this
	double calories;
	int pointsGiven;

	void SnakeCollisionHandler();

public:
	static const FoodInfo ice, celery, normal, donut;

	Food(const Sentinel& prototype, unsigned int size, const FoodInfo& foodInfo);

	void CollisionHandler(WorldObject&) const;
	
	double GetCalories() const;
	int GetPoints() const;
};
