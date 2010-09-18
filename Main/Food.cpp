#include "Food.hpp"
#include "Common.hpp"
#include "Logger.hpp"

#include <cstdio>
#include <cmath>
#include <boost/concept_check.hpp>

Food::FoodInfo::FoodInfo(double _calories, Color24 _color) :
	calories(_calories), color(_color)
{
}

Food::Food(Point location, unsigned int size, const FoodInfo& foodInfo) :
	WorldObject(food), eaten(false), calories(foodInfo.calories)
{
	bounds.min = location;
	bounds.max = Point(location.x + size, location.y + size);

	color = foodInfo.color;
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
