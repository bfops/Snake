#include "Food.hpp"

#include "Common.hpp"

Food::Food(const Point location, const Config::SpawnsData::FoodData& foodData) :
	Spawn(food, location, foodData)
{
}

void Food::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
