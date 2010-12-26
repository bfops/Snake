#pragma once

#include "Direction.hpp"
#include "WorldObject.hpp"

class Food;
struct Line;
class Snake;
struct ZippedUniqueObjectList;

// rectangular segment of snake
class SnakeSegment : public WorldObject
{
private:
	unsigned short width;
	// direction of movement
	Direction direction;

	Snake* parent;

	// change length by _amount_
	void ModifyLength(long amount);

public:
	SnakeSegment(Snake* parent, Point location, Direction direction, unsigned short segmentWidth);
	
	void CollisionHandler(WorldObject&) const;
	void CollisionHandler(const Food&);

	void Grow();
	// return true if the segment became empty
	bool Shrink();

	unsigned long GetLength() const;
	// get direction of travel
	Direction GetDirection() const;
	// get the frontmost [width x width] square of this segment
	Bounds GetHeadSquare() const;

	// get the frontmost side of this segment
	Line GetHeadSide() const;
	// set the frontmost side of this segment
	void SetHeadSide(Line side);
	// get the backmost side of this segment
	Line GetTailSide() const;
	// set the backmost side of this segment
	void SetTailSide(Line side);
};
