#pragma once

#include "WorldObject.hpp"

class Sentinel : public WorldObject
{
private:
public:
	Sentinel(Point location, unsigned int size);
	
	void CollisionHandler(WorldObject&) const;
};
