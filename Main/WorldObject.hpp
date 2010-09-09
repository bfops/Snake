#pragma once

#include <cstdlib>

#include "Color24.hpp"
#include "Point.hpp"
#include "Screen.hpp"

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
	Color24 color;

	virtual ~WorldObject();

	virtual ObjectType GetObjectType() const = 0;
	virtual void CollisionHandler(const WorldObject& colidee) = 0;
	virtual void Draw(Screen& target) const;
};
