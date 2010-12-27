#pragma once

#include "Point.hpp"

struct Vector2D;

// express a vertical or horizontal
struct Line
{
	// (min x, min y)
	Point min;
	unsigned short length;
	// whether or not _length_ is oriented horizontally
	bool horizontal;

	Line& operator+=(Vector2D);
	Line operator+(Vector2D) const;
};
