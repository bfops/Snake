#include "SnakeSegment.hpp"

#include "Common.hpp"
#include "Food.hpp"
#include "Side.hpp"

const double SnakeSegment::HUNGRY = 0;

static const Color24 segmentColor(0, 255, 0);

SnakeSegment::SnakeSegment() :
	WorldObject(snake), direction(Direction::empty)
{
	Init();
}

SnakeSegment::SnakeSegment(Point location, Direction _direction, unsigned int _width) :
	WorldObject(snake), width(_width), direction(_direction)
{
	color = segmentColor;
	bounds.min = location;
	bounds.max = location;

	// account for segment orientation
	if(direction == Direction::left || direction == Direction::right)
		bounds.max.y += width;
	else
		bounds.max.x += width;

	Init();
}

void SnakeSegment::Init()
{
	dead = false;
	empty = false;
	digestionInfo = HUNGRY;
}

void SnakeSegment::DeathCollisionHandler()
{
	dead = true;
}

void SnakeSegment::FoodCollisionHandler(const Food& food)
{
	digestionInfo = food.GetCalories();
}

void SnakeSegment::CollisionHandler(const WorldObject& obj)
{
	WorldObject::ObjectType type = obj.GetObjectType();
	if(type == WorldObject::snake || type == WorldObject::wall)
		DeathCollisionHandler();
	else if(type == WorldObject::food)
		FoodCollisionHandler(*reinterpret_cast<const Food*>(&obj));
}

void SnakeSegment::ModifyLength(int amount)
{
	if(!empty)
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

bool SnakeSegment::IsDead() const
{
	return dead;
}

bool SnakeSegment::IsEmpty() const
{
	return empty;
}

double SnakeSegment::GetDigestionInfo() const
{
	return digestionInfo;
}

void SnakeSegment::Digest()
{
	digestionInfo = HUNGRY;
}
