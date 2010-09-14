#pragma once

#include "Common.hpp"
#include "Vector2D.hpp"

class Direction
{
private:
	Direction(int x, int y);
	Vector2D direction;

public:
	static DEF_CONSTANT(Direction, empty, Direction(0, 0))
	static DEF_CONSTANT(Direction, left, Direction(-1, 0))
	static DEF_CONSTANT(Direction, right, Direction(1, 0))
	static DEF_CONSTANT(Direction, up, Direction(0, -1))
	static DEF_CONSTANT(Direction, down, Direction(0, 1))

	operator Vector2D&();
	Direction operator-() const;
	bool operator==(const Direction&) const;
	bool operator!=(const Direction&) const;
};