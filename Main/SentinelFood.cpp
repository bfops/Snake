#include "SentinelFood.hpp"

#include "Screen.hpp"

SentinelFood::SentinelFood() :
	WorldObject(sentinelFood)
{
}

SentinelFood::SentinelFood(Point location, unsigned int size) :
	WorldObject(sentinelFood)
{
	color = Screen::transparent();
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
