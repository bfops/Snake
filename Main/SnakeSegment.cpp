#include <cassert>

#include "SnakeSegment.hpp"

void SnakeSegment::CollisionHandler(const WorldObject& obj)
{
	WorldObject::ObjectType type = obj.GetObjectType();
	if(type == WorldObject::snake || type == WorldObject::wall)
	{
		dead = true;
	}
	else if(type == WorldObject::food)
	{
		assert(!"Food unimplemented");
	}
}
WorldObject::ObjectType SnakeSegment::GetObjectType() const
{
	return snake;
}

bool SnakeSegment::IsDead() const
{
	return dead;
}