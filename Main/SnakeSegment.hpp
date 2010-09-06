#ifndef __SnakeSegment__
#define __SnakeSegment__

#include "Color24.hpp"
#include "WorldObject.hpp"

class SnakeSegment : public WorldObject
{
private:
	bool dead;

public:
	SnakeSegment();

	ObjectType GetObjectType() const;
	void CollisionHandler(const WorldObject&);

	bool IsDead() const;
};

#endif