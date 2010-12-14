#pragma once

#include "WorldObject.hpp"

class Sentinel;

// a snake-killing mine
class Mine : public WorldObject
{
public:
	Mine(Sentinel minePrototype, unsigned int mineSize);

	void CollisionHandler(WorldObject&) const;
};
