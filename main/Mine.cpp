#include "Mine.hpp"

#include "Config.hpp"
#include "Sentinel.hpp"

Mine::Mine(const Sentinel& sentinel) :
	Spawn(mine, sentinel)
{
}

void Mine::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
