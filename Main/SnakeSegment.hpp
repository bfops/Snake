#pragma once

#include "Color24.hpp"
#include "Food.hpp"
#include "WorldObject.hpp"

class SnakeSegment : public WorldObject
{
private:
	bool dead;
	bool eaten;

	void FoodCollisionHandler(const Food& foodObject);
	void DeathCollisionHandler();

public:
	SnakeSegment();

	ObjectType GetObjectType() const;
	void CollisionHandler(const WorldObject&);

	bool IsDead() const;
	bool HasEaten();
};

