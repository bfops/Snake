#include "Common.hpp"
#include "Wall.hpp"

Wall::Wall() :
	color(255, 0, 0)
{
}
Wall::Wall(Point loc, size_t w, size_t h) :
	color(255, 0, 0)
{
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
