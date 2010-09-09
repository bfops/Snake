#include <cassert>

#include "Common.hpp"
#include "Physics.hpp"
#include "SnakeSegment.hpp"

SnakeSegment::SnakeSegment() :
	dead(false), eaten(false)
{
	height = 15;
	width = 1;
}

void SnakeSegment::DeathCollisionHandler()
{
	dead = true;
}
void SnakeSegment::FoodCollisionHandler(const Food& foodObject)
{
	if(!foodObject.IsEaten())
		eaten = true;
}

void SnakeSegment::CollisionHandler(const WorldObject& obj)
{
	WorldObject::ObjectType type = obj.GetObjectType();
	if(type == WorldObject::snake || type == WorldObject::wall)
		DeathCollisionHandler();
	else if(type == WorldObject::food)
		FoodCollisionHandler(static_cast<const Food&>(obj));
}
WorldObject::ObjectType SnakeSegment::GetObjectType() const
{
	return snake;
}

bool SnakeSegment::IsDead() const
{
	return dead;
}
// as soon as the information's gotten,
// pretend the segment's no longer eaten
bool SnakeSegment::HasEaten()
{
	bool retval = eaten;
	eaten = false;
	return retval;
}
