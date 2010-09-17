#pragma once

#include "Direction.hpp"
#include "Food.hpp"
#include "Side.hpp"
#include "WorldObject.hpp"

class SnakeSegment : public WorldObject
{
private:
	bool dead;
	bool hasEaten;
	bool empty;

	unsigned int width;

	Direction direction;

	void FoodCollisionHandler();
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

	unsigned int GetWidth() const;

	Direction GetDirection() const;
	Bounds GetHeadSquare() const;
	Side GetHeadSide() const;
	void SetHeadSide(Side side);
	Side GetTailSide() const;
	void SetTailSide(Side side);

	bool IsDead() const;
	bool HasEaten() const;
	bool IsEmpty() const;

	void Digest();
};
