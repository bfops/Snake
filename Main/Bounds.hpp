#pragma once

#include "Direction.hpp"
#include "Point.hpp"
#include "Side.hpp"

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
