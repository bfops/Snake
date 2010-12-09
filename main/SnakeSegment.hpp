#pragma once

#include "Direction.hpp"
#include "WorldObject.hpp"

class Food;
struct Side;
class Snake;
struct ZippedUniqueObjectList;

class SnakeSegment : public WorldObject
{
public:
	const static double HUNGRY;

private:
	unsigned int width;
	Direction direction;

	Snake* parent;

	/// change length by _amount_
	void ModifyLength(int amount, ZippedUniqueObjectList& gameObjects);

public:
	SnakeSegment();
	SnakeSegment(Snake* parent, Point location, Direction direction, unsigned int segmentWidth);
	
	void CollisionHandler(WorldObject&) const;
	void CollisionHandler(const Food&);

	void Grow(ZippedUniqueObjectList& gameObjects);
	void Shrink(ZippedUniqueObjectList& gameObjects);

	unsigned int GetWidth() const;

	Direction GetDirection() const;
	Bounds GetHeadSquare() const;
	Side GetHeadSide() const;
	void SetHeadSide(Side side);
	Side GetTailSide() const;
	void SetTailSide(Side side);
};
