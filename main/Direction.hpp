#pragma once

#include "Vector2D.hpp"

// must be either a cardinal direction, or have no length
// can only be copy-constructed from existing directions
// (see static const members), or default-constructed to be empty
class Direction
{
private:
	Direction(int x, int y);
	Vector2D direction;

public:
	Direction();

	static const Direction empty, left, right, up, down;

	operator Vector2D() const;
	// return the opposite direction
	Direction operator-() const;
	bool operator==(const Direction&) const;
	bool operator!=(const Direction&) const;
};
