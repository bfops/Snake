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

void Sentinel::Interfere()
{
	interfering = true;
}

void Sentinel::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}

void Sentinel::CollisionHandler(const Food&)
{
	Interfere();
}

void Sentinel::CollisionHandler(const Mine&)
{
	Interfere();
}

void Sentinel::CollisionHandler(const SnakeSegment&)
{
	Interfere();
}

void Sentinel::CollisionHandler(const Sentinel&)
{
	Interfere();
}

void Sentinel::CollisionHandler(const Wall&)
{
	Interfere();
}

bool Sentinel::IsInterfering() const
{
	return interfering;
}
