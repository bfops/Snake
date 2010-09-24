#include "SentinelFood.hpp"

#include "Screen.hpp"

SentinelFood::SentinelFood() :
	WorldObject(sentinelFood)
{
	Init();
}

SentinelFood::SentinelFood(Point location, unsigned int size) :
	WorldObject(sentinelFood)
{
	bounds.min = (bounds.max = location);
	bounds.max.x += size;
	bounds.max.y += size;

	Init();
}

void SentinelFood::Init()
{
	interfering = false;
}

void SentinelFood::CollisionHandler(const WorldObject& obj)
{
	interfering = true;
}

bool SentinelFood::IsInterfering() const
{
	return interfering;
}
