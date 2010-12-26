#pragma once

#include "Spawn.hpp"

// a snake-killing mine
class Mine : public Spawn
{
public:
	Mine(Point location, unsigned short size, Color24);

	void CollisionHandler(WorldObject&) const;
};
