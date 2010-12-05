#pragma once

#include "WorldObject.hpp"

class Wall : public WorldObject
{
public:
	Wall();
	Wall(Point location, unsigned long width, unsigned long height);

	void CollisionHandler(WorldObject&) const;
};
