#include "Mine.hpp"

#include "Config.hpp"
#include "Sentinel.hpp"

Mine::Mine(const Sentinel& sentinel, const unsigned int size) :
	Spawn(mine, sentinel, size, Config::Get().spawn.color)
{
}

void Mine::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
