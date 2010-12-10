#include "SnakeSegment.hpp"

#include "Food.hpp"
#include "Side.hpp"
#include "Snake.hpp"

const double SnakeSegment::HUNGRY = 0;

static const Color24 segmentColor(0, 255, 0);

SnakeSegment::SnakeSegment() :
	WorldObject(snake, segmentColor), direction(Direction::empty)
{
}

SnakeSegment::SnakeSegment(Snake* const _parent, Point location, Direction _direction, unsigned int _width) :
	WorldObject(snake, segmentColor), direction(_direction)
{
	parent = _parent;
	width = _width;
	bounds.min = location;
	bounds.max = location;

	// account for segment orientation
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

void SnakeSegment::ModifyLength(const int amount, ZippedUniqueObjectList& gameObjects)
{
	if(amount > 0)
	{
		Side headSide = GetHeadSide();
		headSide.ApplyVector(direction, amount);
		SetHeadSide(headSide);
	}
	else
	{
		Side tailSide = GetTailSide();
		tailSide.ApplyVector(direction, -amount);
		SetTailSide(tailSide);
	}

	if(bounds.min.x >= bounds.max.x || bounds.min.y >= bounds.max.y)
		parent->EmptyTailNotify(gameObjects);
}

void SnakeSegment::Grow(ZippedUniqueObjectList& gameObjects)
{
	ModifyLength(1, gameObjects);
}

void SnakeSegment::Shrink(ZippedUniqueObjectList& gameObjects)
{
	ModifyLength(-1, gameObjects);
}

unsigned int SnakeSegment::GetWidth() const
{
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
	// front side of the head square
	Side frontSide = GetHeadSide();
	// back side of the head square; to be a square,
	// this must be _width_ away from _frontSide_
	Side backSide = frontSide;
	backSide.ApplyVector(-direction, width);

	Bounds headSquare;
	headSquare.SetSide(frontSide, direction);
	headSquare.SetSide(backSide, -direction);

	return headSquare;
}

Side SnakeSegment::GetHeadSide() const
{
	return bounds.GetSide(direction);
}

void SnakeSegment::SetHeadSide(Side side)
{
	bounds.SetSide(side, direction);
}

Side SnakeSegment::GetTailSide() const
{
	return bounds.GetSide(-direction);
}

void SnakeSegment::SetTailSide(Side side)
{
	bounds.SetSide(side, -direction);
}
