#include <cassert>

#include "Common.hpp"
#include "Physics.hpp"
#include "SnakeSegment.hpp"

SnakeSegment::SnakeSegment() :
	dead(false)
{
	height = 15;
	width = 1;
}

void SnakeSegment::CollisionHandler(const WorldObject& obj)
{
	WorldObject::ObjectType type = obj.GetObjectType();
	if(type == WorldObject::snake || type == WorldObject::wall)
	{
		dead = true;
	}
	else if(type == WorldObject::food)
	{
		// TODO: food
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
