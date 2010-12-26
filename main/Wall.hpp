#pragma once

#include "WorldObject.hpp"

class Wall : public WorldObject
{
public:
	Wall(const Bounds& wallBounds, Color24);

	void CollisionHandler(WorldObject&) const;
};
