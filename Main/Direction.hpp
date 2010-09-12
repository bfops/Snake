#pragma once

#include "Vector2D.hpp"

class Direction
{
private:
	Direction(int x, int y);
	Vector2D direction;

public:
	// TODO: put these in functions
	const static Direction up, down, left, right;

	Direction()
		: direction(0, 0)
	{}

	operator Vector2D&();
	bool operator==(const Direction&) const;
	bool operator!=(const Direction&) const;
};