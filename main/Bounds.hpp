#pragma once

#include "Point.hpp"

class Direction;
struct Side;

struct Bounds
{
	Point min, max;

	Bounds();
	Bounds(Point min, Point max);
	Bounds(Side);

	operator Side() const;

	Side GetSide(Direction whichSide) const;
	void SetSide(Side sideBounds, Direction whichSide);
};
