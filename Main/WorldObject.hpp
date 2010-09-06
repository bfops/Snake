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
	unsigned int height, width;

	virtual ~WorldObject();

	virtual ObjectType GetObjectType() const = 0;
	virtual void CollisionHandler(const WorldObject& colidee) = 0;
};

#endif