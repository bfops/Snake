#include "Mine.hpp"

Mine::Mine(const Point location, const unsigned short size, const Color24 color) :
	Spawn(mine, location, size, color)
{
}

void Mine::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
