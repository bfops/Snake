#pragma once

#include "Point.hpp"

struct Vector2D;

struct Side
{
	Point min;
	unsigned int dimension;
	bool horizontal;

	/// shift _this_ by _vector_ (scaled by _scale_)
	void ApplyVector(Vector2D vector, unsigned int scale);
};
