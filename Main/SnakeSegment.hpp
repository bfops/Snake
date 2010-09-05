#ifndef __SnakeSegment__
#define __SnakeSegment__

#include "Color24.hpp"
#include "WorldObject.hpp"

class SnakeSegment : public WorldObject
{
private:
	bool dead;

public:
	friend class Snake;

	void CollisionHandler(const WorldObject&);
	ObjectType GetObjectType() const;

	bool IsDead() const;
};

#endif