#include "Wall.hpp"

Wall::Wall(const Bounds& _bounds, const Color24 color) :
	WorldObject(wall, color)
{
	bounds = _bounds;
}

void Wall::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
