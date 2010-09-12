#pragma once

#include "Point.hpp"

struct Bounds
{
	Point min, max;

	Bounds();
	Bounds(Point min, Point max);
};
