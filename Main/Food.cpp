#include "Food.hpp"

#include "Common.hpp"
#include "SentinelFood.hpp"

const Food::FoodInfo Food::ice(-2.5, Color24(0, 0, 255));
const Food::FoodInfo Food::celery(0.3, Color24(127, 255, 127));
const Food::FoodInfo Food::normal(1.0, Color24(0, 255, 255));
const Food::FoodInfo Food::donut(3.0, Color24(200, 0, 0));

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
