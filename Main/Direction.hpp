#pragma once

#include "Vector2D.hpp"

class Direction
{
private:
	Direction(int x, int y);
	Vector2D direction;

public:
	const static Direction up, down, left, right;

	operator Vector2D&();
	bool operator==(const Direction&) const;
	bool operator!=(const Direction&) const;
};