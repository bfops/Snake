#pragma once

#include "WorldObject.hpp"

class Sentinel;

// a snake-killing mine
class Mine : public WorldObject
{
public:
	Mine(const Sentinel& minePrototype, unsigned int mineSize);

	void CollisionHandler(WorldObject&) const;
};
