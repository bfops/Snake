#include "Food.hpp"

#include "Common.hpp"
#include "Sentinel.hpp"

Food::Food(const Sentinel& prototype,
	const Config::SpawnData::FoodData& foodInfo) :
	Spawn(food, prototype, Config::Get().spawn.size, foodInfo.color)
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

long long Food::GetPoints() const
{
	return pointsGiven;
}
