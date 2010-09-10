#include "Food.hpp"

Food::Food() :
	WorldObject(food), eaten(false)
{
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
