#include "Wall.hpp"

#include "Config.hpp"

Wall::Wall(const Config::WallsData::WallData& wallData) :
	WorldObject(wall, wallData.color)
{
	bounds.min = wallData.bounds.min;
	bounds.max = wallData.bounds.max;
}

void Wall::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
