#include "Direction.hpp"
#include "Common.hpp"

Direction Direction::empty()
{
	return Direction(0, 0);
}
Direction Direction::left()
{
	return Direction(-1, 0);
}
Direction Direction::right()
{
	return Direction(1, 0);
}
Direction Direction::up()
{
	return Direction(0, -1);
}
Direction Direction::down()
{
	return Direction(0, 1);
}

Direction::Direction(int x, int y) :
	direction(x, y)
{
}

Direction::operator Vector2D&()
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
	return direction == obj.direction;
}
bool Direction::operator!=(const Direction& obj) const
{
	return !(*this == obj);
}
