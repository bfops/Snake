#ifndef __WorldObject__
#define __WorldObject__

#include <cstdlib>

#include "Point.hpp"

class WorldObject
{
public:
	Point location;
	size_t height, width;

	virtual void CollisionHandler(const WorldObject& colidee) = 0;
};

#endif