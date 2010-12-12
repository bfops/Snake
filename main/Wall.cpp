#include "Wall.hpp"

#include "Color24.hpp"

static const Color24 wallColor(255, 0, 0);

Wall::Wall() :
	WorldObject(wall, wallColor)
{
}

Wall::Wall(Point loc, unsigned long w, unsigned long h) :
	WorldObject(wall, wallColor)
{
	bounds.min = loc;
	bounds.max = Point(loc.x + w - 1, loc.y + h - 1);
}

void Wall::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
