#pragma once

#include "Direction.hpp"
#include "Food.hpp"
#include "WorldObject.hpp"

class SnakeSegment : public World::WorldObject
{
private:
	bool dead;
	bool hasEaten;
	bool empty;

	Direction direction;

	void FoodCollisionHandler(const Food& foodObject);
	void DeathCollisionHandler();

	// change length by _amount_
	void ModifyLength(int amount);

public:
	SnakeSegment();
	SnakeSegment(Point location, Direction direction, unsigned int segmentWidth);

	void CollisionHandler(const WorldObject&);

	// increase length
	SnakeSegment& operator++();
	// increase length
	SnakeSegment operator++(int);
	// decrease length
	SnakeSegment& operator--();
	// decrease length
	SnakeSegment operator--(int);

	Direction GetDirection() const;

	bool IsDead() const;
	bool HasEaten() const;
	bool IsEmpty() const;

	void Digest();
};
