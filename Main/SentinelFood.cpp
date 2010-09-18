#include "SentinelFood.hpp"

#include "Screen.hpp"

SentinelFood::SentinelFood(Point location, unsigned int size) :
	WorldObject(sentinelFood)
{
	color = Color24(255, 0, 254);
	bounds.min = (bounds.max = location);
	bounds.max.x += size;
	bounds.max.y += size;
}

void SentinelFood::CollisionHandler(const WorldObject& obj)
{
	interfering = true;
}

bool SentinelFood::IsInterfering() const
{
	return interfering;
}
