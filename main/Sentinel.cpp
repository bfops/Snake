#include "Sentinel.hpp"

Sentinel::Sentinel(Point location, unsigned int size) :
	WorldObject(sentinel)
{
	bounds.min = (bounds.max = location);
	bounds.max.x += size;
	bounds.max.y += size;
}

void Sentinel::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
