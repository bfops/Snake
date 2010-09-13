#include "Wall.hpp"
#include "Color24.hpp"
#include "Common.hpp"

namespace {
DEF_CONSTANT(Color24, defaultColor, Color24(255, 0, 0))
}

Wall::Wall() :
	WorldObject(wall)
{
	color = defaultColor();
}
Wall::Wall(Point loc, size_t w, size_t h) :
	WorldObject(wall)
{
	color = defaultColor();
	bounds.min = loc;
	bounds.max = Point(loc.x + w - 1, loc.y + h - 1);
}

void Wall::CollisionHandler(const WorldObject& obj)
{
}
