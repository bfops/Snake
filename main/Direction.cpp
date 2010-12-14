#include "Direction.hpp"

const Direction Direction::empty(0, 0);
const Direction Direction::left(-1, 0);
const Direction Direction::right(1, 0);
const Direction Direction::up(0, -1);
const Direction Direction::down(0, 1);

Direction::Direction(const int x, const int y)
{
	direction = Vector2D(x, y);
}

Direction::Direction()
{
	direction = Vector2D(empty);
}

Direction::operator Vector2D() const
{
	return direction;
}

Direction Direction::operator-() const
{
	Vector2D nDirection = -direction;
	return Direction(nDirection.x, nDirection.y);
}

bool Direction::operator==(const Direction& obj) const
{
	return (direction == obj.direction);
}

bool Direction::operator!=(const Direction& obj) const
{
	return !(*this == obj);
}
