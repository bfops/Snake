#ifndef __WorldObject__
#define __WorldObject__

#include <cstdlib>

#include "Point.hpp"

class WorldObject
{
public:
	enum ObjectType
	{
		generic,
		snake,
		wall,
		food
	};

	Point location;
	size_t height, width;

	virtual ObjectType GetObjectType() const;
	virtual void CollisionHandler(const WorldObject& colidee) = 0;
};

#endif