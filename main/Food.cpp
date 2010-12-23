#include "Food.hpp"

#include "Common.hpp"
#include "Sentinel.hpp"

Food::Food(const Sentinel& prototype, const Config::SpawnsData::FoodData& foodData) :
	Spawn(food, prototype, Config::Get().spawns.size, foodData.color), info(foodData)
{
}

void Food::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}

const Config::SpawnsData::FoodData& Food::GetData() const
{
	return info;
}
