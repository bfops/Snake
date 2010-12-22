#pragma once

#include "Spawn.hpp"

class Sentinel;

// a snake-killing mine
class Mine : public Spawn
{
public:
	Mine(const Sentinel& minePrototype);

	void CollisionHandler(WorldObject&) const;
};
