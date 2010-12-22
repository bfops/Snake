#include "Mine.hpp"

#include "Config.hpp"
#include "Sentinel.hpp"

Mine::Mine(const Sentinel& sentinel) :
	Spawn(mine, sentinel, Config::Get().spawn.size, Config::Get().spawn.mineColor)
{
}

void Mine::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
