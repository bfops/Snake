#include "Mine.hpp"

static Color24 mineColor(255, 0, 255);

Mine::Mine(Sentinel sentinel, const unsigned int size) :
	WorldObject(mine)
{
	color = mineColor;

	bounds.min = sentinel.GetBounds().min;
	bounds.min.x += size / 2;
	bounds.min.y += size / 2;
	bounds.max = bounds.min;
	bounds.max.x += size;
	bounds.max.y += size;
}

void Mine::CollisionHandler(const WorldObject&)
{
}
