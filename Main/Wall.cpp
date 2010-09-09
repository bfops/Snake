#include "Color24.hpp"
#include "Common.hpp"
#include "Wall.hpp"

Color24 defaultColor(255, 0, 0);
Wall::Wall()
{
	color = defaultColor;
}
Wall::Wall(Point loc, size_t w, size_t h)
{
	color = defaultColor;
	location = loc;
	width = w;
	height = h;
}

WorldObject::ObjectType Wall::GetObjectType() const
{
	return wall;
}
void Wall::CollisionHandler(const WorldObject& obj)
{
}
