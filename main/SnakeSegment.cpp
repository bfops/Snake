#include "SnakeSegment.hpp"

#include "Common.hpp"
#include "Config.hpp"
#include "Line.hpp"
#include "Snake.hpp"

SnakeSegment::SnakeSegment(Snake* const _parent, const Point location, const Direction _direction,
	const unsigned short _width) :
	WorldObject(snake, Config::Get().snake.color)
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

void SnakeSegment::ModifyLength(const long amount)
{
	DOLOCKED(mutex,
		const Vector2D v = direction;
		if(amount > 0)
		{
			// move the front side forward by _amount_ (i.e. grow)
			SetHeadSide(GetHeadSide() + (v * amount));
		}
		else
		{
			// move the back side forward by _amount_ (i.e. shrink)
			SetTailSide(GetTailSide() + (v * -amount));
		}
	)
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

unsigned long SnakeSegment::GetLength() const
{
#define DELTA(m) (bounds.max.m - bounds.min.m)
	// if moving horizontally, the length is delta X, otherwise delta y
	if(direction == Direction::left || direction == Direction::right)
		return DELTA(x);

	return DELTA(y);
#undef DELTA
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
