#include "Direction.hpp"

const Direction Direction::up(0, -1);
const Direction Direction::down(0, 1);
const Direction Direction::left(-1, 0);
const Direction Direction::right(1, 0);

Direction::Direction(int x, int y) :
	direction(x, y)
{
}

Direction::operator Vector2D&()
{
	return direction;
}
bool Direction::operator==(const Direction& obj) const
{
	return direction == obj.direction;
}
bool Direction::operator!=(const Direction& obj) const
{
	return !(*this == obj);
}
