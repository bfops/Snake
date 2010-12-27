#pragma once

#include "Direction.hpp"
#include "WorldObject.hpp"

class Food;
struct Line;
class Snake;
struct ZippedUniqueObjectCollection;

// rectangular segment of snake
class SnakeSegment : public WorldObject
{
private:
	unsigned short width;

	Snake* parent;

	// change length by _amount_
	void ModifyLength(long amount);

public:
	// direction of movement
	Direction direction;

	SnakeSegment(Snake* parent, Point location, Direction direction, unsigned long length,
		unsigned short width, Color24);
	
	void CollisionHandler(WorldObject&) const;
	void CollisionHandler(const Food&);

	// move one step in the current direction
	void Move();

	void Grow();
	// return true if the segment became empty
	bool Shrink();

	unsigned long GetLength() const;

	// get the frontmost side of this segment
	Line GetHeadSide() const;
	// set the frontmost side of this segment
	void SetHeadSide(Line side);
	// get the backmost side of this segment
	Line GetTailSide() const;
	// set the backmost side of this segment
	void SetTailSide(Line side);
};
