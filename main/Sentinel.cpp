#include "Sentinel.hpp"

Sentinel::Sentinel(const Point location, const unsigned int size) :
	WorldObject(sentinel)
{
	bounds.min = bounds.max = location;
	bounds.max.x += size;
	bounds.max.y += size;
}

void Sentinel::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
