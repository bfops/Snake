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

	// shift _this_ by normalized _vector_, _scale_ times
	void ApplyVector(Vector2D vector, unsigned long scale);
};
