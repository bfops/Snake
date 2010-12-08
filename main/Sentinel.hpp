#pragma once

#include "WorldObject.hpp"

// TODO: come up with a different sentinel collision
// mechanism; using the physics engine as-is will
// start to become cumbersome
class Sentinel : public WorldObject
{
private:
public:
	Sentinel(Point location, unsigned int size);
	
	void CollisionHandler(WorldObject&) const;
};
