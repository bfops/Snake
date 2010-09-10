#pragma once

#include "Point.hpp"
#include "WorldObject.hpp"

class Wall : public World::WorldObject
{
public:
	Wall();
	Wall(Point location, size_t width, size_t height);

	void CollisionHandler(const WorldObject&);
};
