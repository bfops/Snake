#pragma once

#include "Direction.hpp"
#include "Point.hpp"

struct Bounds
{
	Point min, max;

	Bounds();
	Bounds(Point min, Point max);

	Bounds GetSide(Direction whichSide);
	void SetSide(Bounds sideBounds, Direction whichSide);
};
