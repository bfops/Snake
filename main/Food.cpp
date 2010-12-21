#include "Food.hpp"

#include "Common.hpp"
#include "Sentinel.hpp"

Food::Food(const Sentinel& prototype, const unsigned int size,
	const Config::SpawnData::FoodData& foodInfo) :
	Spawn(food, prototype, size, foodInfo.color)
{
	calories = foodInfo.lengthFactor;
	pointsGiven = foodInfo.points;
}

void Food::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}

double Food::GetCalories() const
{
	return calories;
}

int Food::GetPoints() const
{
	return pointsGiven;
}
