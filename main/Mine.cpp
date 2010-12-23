#include "Mine.hpp"

#include "Config.hpp"
#include "Sentinel.hpp"

Mine::Mine(const Sentinel& sentinel) :
	Spawn(mine, sentinel, Config::Get().spawns.size, Config::Get().spawns.mine.color)
{
}

void Mine::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
