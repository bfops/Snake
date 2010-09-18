#include "Food.hpp"

#include "Common.hpp"

Food::FoodInfo::FoodInfo(double _calories, Color24 _color) :
	calories(_calories), color(_color)
{
}

Food::Food(SentinelFood& prototype, const FoodInfo& foodInfo) :
	WorldObject(food), eaten(false)
{
	bounds = prototype.GetBounds();
	color = foodInfo.color;
	calories = foodInfo.calories;
}

void Food::SnakeCollisionHandler()
{
	eaten = true;
}

void Food::CollisionHandler(const WorldObject& obj)
{
	if(obj.GetObjectType() == snake)
		SnakeCollisionHandler();
}

bool Food::IsEaten() const
{
	return eaten;
}

double Food::GetCalories() const
{
	return calories;
}
