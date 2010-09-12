#include "SnakeSegment.hpp"
#include "Common.hpp"

SnakeSegment::SnakeSegment() :
	WorldObject(snake)
{
}
SnakeSegment::SnakeSegment(Point location, Direction _direction, unsigned int width) :
	WorldObject(snake), dead(false), hasEaten(false), direction(_direction)
{
	color = Color24(0, 255, 0);
	minBounds = location;
	maxBounds = location;

	if(direction == Direction::left || direction == Direction::right)
		maxBounds.y += width;
	else
		maxBounds.x += width;
}

void SnakeSegment::DeathCollisionHandler()
{
	dead = true;
}
void SnakeSegment::FoodCollisionHandler(const Food& foodObject)
{
	if(!foodObject.IsEaten())
		hasEaten = true;
}

void SnakeSegment::CollisionHandler(const WorldObject& obj)
{
	WorldObject::ObjectType type = obj.GetObjectType();
	if(type == WorldObject::snake || type == WorldObject::wall)
		DeathCollisionHandler();
	else if(type == WorldObject::food)
		FoodCollisionHandler(reinterpret_cast<const Food&>(obj));
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
			minBounds.x -= amount;
		else if(direction == Direction::right)
			maxBounds.x += amount;
		else if(direction == Direction::up)
			minBounds.y -= amount;
		else if(direction == Direction::down)
			maxBounds.y += amount;
	}
	else
	{
		if(direction == Direction::left)
			maxBounds.x += amount;
		else if(direction == Direction::right)
			minBounds.x -= amount;
		else if(direction == Direction::up)
			maxBounds.y += amount;
		else if(direction == Direction::down)
			minBounds.y -= amount;
	}
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

bool SnakeSegment::IsDead() const
{
	return dead;
}
// as soon as the information's gotten,
// pretend the segment's no longer eaten
bool SnakeSegment::HasEaten()
{
	bool retval = hasEaten;
	hasEaten = false;
	return retval;
}
