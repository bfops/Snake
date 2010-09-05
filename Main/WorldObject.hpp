#ifndef __WorldObject__
#define __WorldObject__

#include <cstdlib>

#include "Point.hpp"

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
	size_t height, width;

	virtual ObjectType GetObjectType() const = 0;
	virtual void CollisionHandler(const WorldObject& colidee) = 0;
};

#endif