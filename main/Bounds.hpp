#pragma once

#include "Point.hpp"

class Direction;
struct Side;

// define the bounds of a rectangle
struct Bounds
{
	Point min, max;

	Bounds();
	Bounds(Point min, Point max);
	Bounds(Side);

	operator Side() const;
	
	// get the _whichSide_ side of this rectangle
	Side GetSide(Direction whichSide) const;
	// set the _whichSide_ side of this rectangle
	void SetSide(Side sideBounds, Direction whichSide);
};
