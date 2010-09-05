#ifndef __SnakeSegment__
#define __SnakeSegment__

#include "Color24.hpp"
#include "WorldObject.hpp"

class SnakeSegment : public WorldObject
{
public:
	friend class Snake;

	void CollisionHandler(const WorldObject&);
};

#endif