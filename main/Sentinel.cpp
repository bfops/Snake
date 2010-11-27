#include "Sentinel.hpp"

Sentinel::Sentinel(Point location, unsigned int size) :
	WorldObject(sentinel)
{
	bounds.min = (bounds.max = location);
	bounds.max.x += size;
	bounds.max.y += size;

	Init();
}

void Sentinel::Init()
{
	interfering = false;
}

void Sentinel::CollisionHandler(const WorldObject&)
{
	interfering = true;
}

bool Sentinel::IsInterfering() const
{
	return interfering;
}
