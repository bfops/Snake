#include "Wall.hpp"
#include "Color24.hpp"
#include "Common.hpp"

namespace {
Color24 defaultColor(255, 0, 0);
}

Wall::Wall() :
	WorldObject(wall)
{
	color = defaultColor;
}
Wall::Wall(Point loc, size_t w, size_t h) :
	WorldObject(wall)
{
	color = defaultColor;
	minBounds = loc;
	maxBounds = Point(loc.x + w - 1, loc.y + h - 1);
}

void Wall::CollisionHandler(const WorldObject& obj)
{
}
