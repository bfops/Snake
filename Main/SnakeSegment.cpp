#include <cassert>

#include "Physics.hpp"
#include "SnakeSegment.hpp"

// TODO: the segments shouldn't be responsible
// for their Physics handling, but if they're
// not, then vector allocations cause the
// pointers in the physics world to be invalid.
SnakeSegment::SnakeSegment() :
	dead(false)
{
	height = 1;
	width = 1;
	location = Vector2D(-1, -1);
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
