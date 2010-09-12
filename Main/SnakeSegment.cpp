#include "SnakeSegment.hpp"
#include "Common.hpp"
#include "Logger.hpp"

// URGENT TODO: create a side-getting function for a Bound,
// which returns the bounds of the side gotten,
// e.g. Bounds get_side(Bounds b, Direction whichSide)

namespace {
Logger::Handle logger = Logger::RequestHandle("SnakeSegment");
DEF_CONSTANT(Color24, segmentColor, Color24(0, 255, 0))
}

SnakeSegment::SnakeSegment() :
	WorldObject(snake), dead(false), hasEaten(false), empty(false)
{
}
SnakeSegment::SnakeSegment(Point location, Direction _direction, unsigned int width) :
	WorldObject(snake), dead(false), hasEaten(false), empty(false), direction(_direction)
{
	color = segmentColor();
	bounds.min = location;
	bounds.max = location;

	if(direction == Direction::left || direction == Direction::right)
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
	// TODO: MAKE LESS UGLY.
	// basically with negative amounts,
	// the opposite bound gets the opposite
	// operation.
	// If amount is negated, then only the
	// bound is different between the two IFs
	if(amount > 0)
	{
		if(direction == Direction::left)
			bounds.min.x -= amount;
		else if(direction == Direction::right)
			bounds.max.x += amount;
		else if(direction == Direction::up)
			bounds.min.y -= amount;
		else if(direction == Direction::down)
			bounds.max.y += amount;
	}
	else
	{
		if(direction == Direction::left)
			bounds.max.x += amount;
		else if(direction == Direction::right)
			bounds.min.x -= amount;
		else if(direction == Direction::up)
			bounds.max.y += amount;
		else if(direction == Direction::down)
			bounds.min.y -= amount;
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
