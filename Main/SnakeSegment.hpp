#pragma once

#include "Food.hpp"
#include "WorldObject.hpp"

class SnakeSegment : public World::WorldObject
{
private:
	bool dead;
	bool eaten;

	void FoodCollisionHandler(const Food& foodObject);
	void DeathCollisionHandler();

public:
	SnakeSegment();

	//ObjectType GetObjectType() const;
	void CollisionHandler(const WorldObject&);

	bool IsDead() const;
	bool HasEaten();
};

