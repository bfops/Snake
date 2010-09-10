#pragma once

#include "WorldObject.hpp"

// TODO: different food types
class Food : public WorldObject
{
private:
	bool eaten;

	void SnakeCollisionHandler();

public:
	Food();

	//ObjectType GetObjectType() const;
	void CollisionHandler(const WorldObject&);

	bool IsEaten() const;
};
