#include "Mine.hpp"

#include "Config.hpp"

Mine::Mine(const Point location, const Config::SpawnsData::MineData& mineData) :
	Spawn(mine, location, mineData)
{
}

void Mine::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
