#include "Wall.hpp"

Wall::Wall()
{
}
Wall::Wall(Point loc, size_t w, size_t h)
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