#pragma once

#include "Point.hpp"

class Direction;
struct Line;

// define the bounds of a rectangle
struct Bounds
{
	Point min, max;

	Bounds();
	Bounds(Point min, Point max);
	Bounds(Line);

	operator Line() const;
	
	// get the _whichSide_ side of this rectangle
	Line GetSide(Direction whichSide) const;
	// set the _whichSide_ side of this rectangle
	void SetSide(Line sideBounds, Direction whichSide);
};
