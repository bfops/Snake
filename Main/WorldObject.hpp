#pragma once

#include <cstdlib>

#include "Point.hpp"

// TODO: use sprites
// TODO: make this be a generic object for
// both Physics and Graphics worlds
class WorldObject
{
public:
	enum ObjectType
	{
		snake,
		wall,
		food
	};

	Point location;
	unsigned int height, width;

	virtual ~WorldObject();

	virtual ObjectType GetObjectType() const = 0;
	virtual void CollisionHandler(const WorldObject& colidee) = 0;
};
