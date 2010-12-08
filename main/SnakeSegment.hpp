#pragma once

#include "Direction.hpp"
#include "WorldObject.hpp"

class Food;
struct Side;
class Snake;

class SnakeSegment : public WorldObject
{
public:
	const static double HUNGRY;

private:
	// TODO: event-drive this
	bool empty;

	unsigned int width;
	Direction direction;

	Snake* parent;

	void Init();

	/// change length by _amount_
	void ModifyLength(int amount);

public:
	SnakeSegment();
	SnakeSegment(Snake* parent, Point location, Direction direction, unsigned int segmentWidth);
	
	void CollisionHandler(WorldObject&) const;
	void CollisionHandler(const Food&);

	/// increase length
	SnakeSegment& operator++();
	/// increase length
	SnakeSegment operator++(int);
	/// decrease length
	SnakeSegment& operator--();
	/// decrease length
	SnakeSegment operator--(int);

	unsigned int GetWidth() const;

	Direction GetDirection() const;
	Bounds GetHeadSquare() const;
	Side GetHeadSide() const;
	void SetHeadSide(Side side);
	Side GetTailSide() const;
	void SetTailSide(Side side);

	bool IsEmpty() const;
};
