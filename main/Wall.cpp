#include "Wall.hpp"

#include "Config.hpp"

Wall::Wall() :
	WorldObject(wall, Config::Get().wallsData.color)
{
}

Wall::Wall(const Point loc, const unsigned long w, const unsigned long h) :
	WorldObject(wall, Config::Get().wallsData.color)
{
	bounds.min = loc;
	bounds.max = Point(loc.x + w - 1, loc.y + h - 1);
}

void Wall::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
