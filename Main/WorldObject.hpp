#ifndef __WorldObject__
#define __WorldObject__

#include "Point.hpp"

class WorldObject
{
public:
	Point location;
	size_t height, width;

	virtual void CollisionHandler(const WorldObject& colidee);
};

#endif