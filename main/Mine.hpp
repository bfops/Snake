#pragma once

#include "WorldObject.hpp"

class Sentinel;

class Mine : public WorldObject
{
public:
	Mine(Sentinel minePrototype, unsigned int size);

	void CollisionHandler(WorldObject&) const;
};
