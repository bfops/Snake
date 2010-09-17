#include "Food.hpp"
#include "Common.hpp"

#include <cstdio>

namespace {
DEF_CONSTANT(Color24, foodColor, Color24(0, 255, 255))
}

Food::Food(Point location, unsigned int size) :
	WorldObject(food), eaten(false)
{
	bounds.min = location;
	bounds.max = Point(location.x + size, location.y + size);

	color = foodColor();
}

void Food::SnakeCollisionHandler()
{
	printf("hello!");
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
