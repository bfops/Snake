#pragma once

#include "Point.hpp"
#include "WorldObject.hpp"

// TODO: different food types
class Food : public World::WorldObject
{
private:
	bool eaten;

	void SnakeCollisionHandler();

public:
	Food(Point location, unsigned int size);

	//ObjectType GetObjectType() const;
	void CollisionHandler(const WorldObject&);

	bool IsEaten() const;
};
