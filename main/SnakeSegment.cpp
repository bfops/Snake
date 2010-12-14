#include "SnakeSegment.hpp"

#include "Line.hpp"
#include "Snake.hpp"

static const Color24 segmentColor(0, 255, 0);

SnakeSegment::SnakeSegment() :
	WorldObject(snake, segmentColor)
{
}

SnakeSegment::SnakeSegment(Snake* const _parent, const Point location,
	const Direction _direction, const unsigned int _width) :
	WorldObject(snake, segmentColor)
{
	direction = _direction;
	parent = _parent;
	width = _width;
	bounds.min = location;
	bounds.max = location;

	// if it's moving horizontally, its width is vertical
	if(direction == Direction::left || direction == Direction::right)
		bounds.max.y += width;
	else
		bounds.max.x += width;
}

void SnakeSegment::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}

void SnakeSegment::CollisionHandler(const Food& food)
{
	parent->EatFood(food);
}

void SnakeSegment::ModifyLength(const int amount)
{
	if(amount > 0)
	{
		// move the front side forward by _amount_ (i.e. grow)
		Line headSide = GetHeadSide();
		headSide.ApplyVector(direction, amount);
		SetHeadSide(headSide);
	}
	else
	{
		// move the back side forward by _amount_ (i.e. shrink)
		Line tailSide = GetTailSide();
		tailSide.ApplyVector(direction, -amount);
		SetTailSide(tailSide);
	}
}

void SnakeSegment::Grow()
{
	ModifyLength(1);
}

bool SnakeSegment::Shrink()
{
	ModifyLength(-1);

	// if bounds are exceeded, this segment is empty
	return (bounds.min.x >= bounds.max.x || bounds.min.y >= bounds.max.y);
}

unsigned int SnakeSegment::GetLength() const
{
	// if moving horizontally, the length is delta X, otherwise delta y
	if(direction == Direction::left || direction == Direction::right)
		return bounds.max.x - bounds.min.x - 1;

	return bounds.max.y - bounds.min.y - 1;
}

Direction SnakeSegment::GetDirection() const
{
	return direction;
}

Bounds SnakeSegment::GetHeadSquare() const
{
	const Line frontSide = GetHeadSide();
	// back side of the head square; to be a square,
	// this must be _width_ away from _frontSide_
	Line backSide = frontSide;
	backSide.ApplyVector(-direction, width);

	Bounds headSquare;
	headSquare.SetSide(frontSide, direction);
	headSquare.SetSide(backSide, -direction);

	return headSquare;
}

Line SnakeSegment::GetHeadSide() const
{
	// the _direction_ side is the front
	return bounds.GetSide(direction);
}

void SnakeSegment::SetHeadSide(Line side)
{
	bounds.SetSide(side, direction);
}

Line SnakeSegment::GetTailSide() const
{
	// the -_direction_ side is the back
	return bounds.GetSide(-direction);
}

void SnakeSegment::SetTailSide(Line side)
{
	bounds.SetSide(side, -direction);
}
