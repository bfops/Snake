#include "SnakeSegment.hpp"
#include "Common.hpp"
#include "Logger.hpp"

namespace {
DEF_CONSTANT(Color24, segmentColor, Color24(0, 255, 0))
}

SnakeSegment::SnakeSegment() :
	WorldObject(snake), dead(false), hasEaten(false), empty(false), direction(Direction::empty())
{
}
SnakeSegment::SnakeSegment(Point location, Direction _direction, unsigned int _width) :
	WorldObject(snake), dead(false), hasEaten(false), empty(false), width(_width), direction(_direction)
{
	color = segmentColor();
	bounds.min = location;
	bounds.max = location;

	// account for segment orientation
	if(direction == Direction::left() || direction == Direction::right())
		bounds.max.y += width;
	else
		bounds.max.x += width;
}

void SnakeSegment::DeathCollisionHandler()
{
	dead = true;
}
void SnakeSegment::FoodCollisionHandler()
{
	hasEaten = true;
}

void SnakeSegment::CollisionHandler(const WorldObject& obj)
{
	WorldObject::ObjectType type = obj.GetObjectType();
	if(type == WorldObject::snake || type == WorldObject::wall)
		DeathCollisionHandler();
	else if(type == WorldObject::food)
		FoodCollisionHandler();
}

void SnakeSegment::ModifyLength(int amount)
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
		empty = true;
}

SnakeSegment& SnakeSegment::operator++()
{
	ModifyLength(1);
	return *this;
}
SnakeSegment SnakeSegment::operator++(int)
{
	SnakeSegment returnvalue(*this);
	++(*this);

	return returnvalue;
}
SnakeSegment& SnakeSegment::operator--()
{
	ModifyLength(-1);
	return *this;
}
SnakeSegment SnakeSegment::operator--(int)
{
	SnakeSegment returnvalue(*this);
	--(*this);

	return returnvalue;
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

bool SnakeSegment::IsDead() const
{
	return dead;
}
// as soon as the information's gotten,
// pretend the segment's no longer eaten
bool SnakeSegment::HasEaten() const
{
	return hasEaten;
}
bool SnakeSegment::IsEmpty() const
{
	return empty;
}
void SnakeSegment::Digest()
{
	hasEaten = false;
}
