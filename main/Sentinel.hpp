#pragma once

#include "WorldObject.hpp"

// a square sentinel to check for collisions
class Sentinel : public WorldObject
{
public:
	Sentinel(Point location, unsigned int size);
	
	void CollisionHandler(WorldObject&) const;
};
