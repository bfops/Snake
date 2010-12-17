#include "Mine.hpp"

#include "Sentinel.hpp"

static Color24 mineColor(255, 0, 255);

Mine::Mine(const Sentinel& sentinel, const unsigned int size) :
	Spawn(mine, sentinel, size, mineColor)
{
}

void Mine::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
