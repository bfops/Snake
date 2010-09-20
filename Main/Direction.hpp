#pragma once

#include "Common.hpp"
#include "Vector2D.hpp"

class Direction
{
private:
	Direction(int x, int y);
	Vector2D direction;

public:
	static const Direction empty, left, right, up, down;

	operator Vector2D() const;
	Direction operator-() const;
	bool operator==(const Direction&) const;
	bool operator!=(const Direction&) const;
};
