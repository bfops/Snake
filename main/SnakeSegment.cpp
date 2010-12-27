#include "SnakeSegment.hpp"

#include "Common.hpp"
#include "Config.hpp"
#include "Line.hpp"
#include "Snake.hpp"

SnakeSegment::SnakeSegment(Snake* const _parent, const Point location, const Direction _direction,
	const unsigned long length, const unsigned short _width, const Color24 color) :
	WorldObject(snake, color)
{
	direction = _direction;
	parent = _parent;
	width = _width;
	bounds.min = location;
	bounds.max = location;

	Vector2D size;
	// if it's moving horizontally, its width is vertical
	if(direction.IsHorizontal())
		size = Vector2D(length, width);
	else
		size = Vector2D(width, length);

	bounds.max += size;
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

void SnakeSegment::Move()
{
	bounds += direction;
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
	if(direction.IsHorizontal())
		return DELTA(x);

	return DELTA(y);
#undef DELTA
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
