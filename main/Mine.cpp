#include "Mine.hpp"

static Color24 mineColor(255, 0, 255);

Mine::Mine(Sentinel sentinel, const unsigned int size) :
	WorldObject(mine)
{
	color = mineColor;

	const unsigned int prototypeSize = sentinel.GetBounds().max.x - sentinel.GetBounds().min.x;
	const unsigned int sizeDiff = prototypeSize - size;
	bounds.min = sentinel.GetBounds().min;
	bounds.min.x += sizeDiff / 2;
	bounds.min.y += sizeDiff / 2;
	bounds.max = bounds.min;
	bounds.max.x += size;
	bounds.max.y += size;
}

void Mine::CollisionHandler(const WorldObject&)
{
}
