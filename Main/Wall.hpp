#pragma once

#include "Point.hpp"
#include "WorldObject.hpp"

class Wall : public WorldObject
{
public:
	Wall();
	Wall(Point location, size_t width, size_t height);

	ObjectType GetObjectType() const;
	void CollisionHandler(const WorldObject&);
};
