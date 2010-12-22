#include "Food.hpp"

#include "Common.hpp"
#include "Sentinel.hpp"

Food::Food(const Sentinel& prototype, const Config::SpawnData::FoodData& foodData) :
	Spawn(food, prototype, Config::Get().spawn.size, foodData.color), info(foodData)
{
}

void Food::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}

const Config::SpawnData::FoodData& Food::GetData() const
{
	return info;
}
