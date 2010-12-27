#pragma once

#include "Vector2D.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <SDL_types.h>

#ifdef MSVC
#pragma warning(pop)
#endif

// must be either a cardinal direction, or have no length
// can only be copy-constructed from existing directions
// (see static const members), or default-constructed to be empty
class Direction
{
private:
	Direction(Sint8 x, Sint8 y);
	Vector2D direction;

public:
	Direction();

	static const Direction empty, left, right, up, down;

	operator Vector2D() const;

	// return the opposite direction
	Direction operator-() const;

	bool operator==(Direction) const;

	bool IsHorizontal() const;
};
