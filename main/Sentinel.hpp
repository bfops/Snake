#pragma once

#include "WorldObject.hpp"

// Sentinels are sent out before spawns to check for collisions
// and thus clear a small (square) area for spawns
class Sentinel : public WorldObject
{
public:
	Sentinel(Point location, unsigned short size);
	
	void CollisionHandler(WorldObject&) const;
};
