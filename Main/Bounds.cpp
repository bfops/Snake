#include "Bounds.hpp"

#include <cassert>

Bounds::Bounds()
{
}
Bounds::Bounds(Point _min, Point _max) :
	min(_min), max(_max)
{
}
Bounds::Bounds(Side side) :
	min(side.min), max(side.min)
{
	if(side.horizontal)
		max.x += side.dimension;
	else
		max.y += side.dimension;
}

Bounds::operator Side() const
{
	// it's gotta be a line (i.e. one dimension is 0)
	// in order to be a side
	assert(min.x == max.x || min.y == max.y);

	Side retval;
	retval.min = min;

	if(min.x == max.x)
	{
		retval.horizontal = false;
		retval.dimension = (max.y - min.y);
	}
	else
	{
		retval.horizontal = true;
		retval.dimension = (max.x - min.x);
	}

	return retval;
}

namespace {
inline bool validDirection(Direction direction)
{
	return (direction == Direction::left() || direction == Direction::right()
		|| direction == Direction::up() || direction == Direction::down());
}

void transfer_side(const Bounds& input, Bounds& output, Direction whichSide)
{
	assert(validDirection(whichSide));
	if(whichSide == Direction::left())
	{
		output.min = input.min;
		output.max.y = input.max.y;
	}
	else if(whichSide == Direction::right())
	{
		output.min.y = input.min.y;
		output.max = input.max;
	}
	else if(whichSide == Direction::up())
	{
		output.min = input.min;
		output.max.x = input.max.x;
	}
	else
	{
		output.min.x = input.min.x;
		output.max = input.max;
	}
}
}

Side Bounds::GetSide(Direction whichSide) const
{
	assert(validDirection(whichSide));

	Bounds retval;
	transfer_side(*this, retval, whichSide);

	// getting a side from a bounded rectangle
	// is "lossy", so we need to set mins = maxs
	// or maxs = mins to "lose" the data
	if(whichSide == Direction::left())
		retval.max.x = retval.min.x;
	else if(whichSide == Direction::right())
		retval.min.x = retval.max.x;
	else if(whichSide == Direction::up())
		retval.max.y = retval.min.y;
	else
		retval.min.y = retval.max.y;

	return retval;
}
void Bounds::SetSide(Side side, Direction whichSide)
{
	assert(validDirection(whichSide));
	transfer_side(side, *this, whichSide);
}
