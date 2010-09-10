#include "Color24.hpp"
#include "Common.hpp"
#include "Wall.hpp"

Color24 defaultColor(255, 0, 0);
Wall::Wall() :
	WorldObject(wall)
{
	color = defaultColor;
}
Wall::Wall(Point loc, size_t w, size_t h) :
	WorldObject(wall)
{
	color = defaultColor;
	location = loc;
	width = w;
	height = h;
}

void Wall::CollisionHandler(const WorldObject& obj)
{
}
