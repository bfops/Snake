#include "Food.hpp"

Food::Food() :
	eaten(false)
{
}

void Food::SnakeCollisionHandler()
{
	eaten = true;
}

WorldObject::ObjectType Food::GetObjectType() const
{
	return food;
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
