#include "Food.hpp"

#include "Common.hpp"
#include "Sentinel.hpp"

Food::Food(const Sentinel& prototype) :
	Spawn(food, prototype)
{
}

void Food::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}
