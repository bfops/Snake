#pragma once

#include "WorldObject.hpp"

class Wall : public WorldObject
{
public:
	Wall();
	Wall(Point location, size_t width, size_t height);

	void CollisionHandler(const WorldObject&);
};
