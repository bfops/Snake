#pragma once

#include "Sentinel.hpp"
#include "WorldObject.hpp"

class Mine : public WorldObject
{
public:
	Mine(Sentinel minePrototype, unsigned int size);

	void CollisionHandler(const WorldObject&);
};
