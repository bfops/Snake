#include "Bounds.hpp"

#include "Direction.hpp"
#include "Line.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <cassert>

#ifdef MSVC
#pragma warning(pop)
#endif

Bounds::Bounds()
{
}

Bounds::Bounds(Point _min, Point _max) :
	min(_min), max(_max)
{
}

Bounds::Bounds(Line side) :
	min(side.min), max(side.min)
{
	if(side.horizontal)
		max.x += side.length;
	else
		max.y += side.length;
}

Bounds::operator Line() const
{
	// it's gotta be a line (i.e. one dimension is 0)
	// in order to be a side
	assert(min.x == max.x || min.y == max.y);

	Line retval;
	retval.min = min;

#define LOADDIFF(m) retval.length = max.m - min.m;
	if(min.x == max.x)
	{
		retval.horizontal = false;
		LOADDIFF(y)
	}
	else
	{
		retval.horizontal = true;
		LOADDIFF(x)
	}

	return retval;
}

static inline bool validDirection(const Direction direction)
{
	return (direction == Direction::left || direction == Direction::right
		|| direction == Direction::up || direction == Direction::down);
}

static void transfer_side(const Bounds& input, Bounds& output, const Direction whichSide)
{
	assert(validDirection(whichSide));
	if(whichSide == Direction::left)
	{
		output.min = input.min;
		output.max.y = input.max.y;
	}
	else if(whichSide == Direction::right)
	{
		output.min.y = input.min.y;
		output.max = input.max;
	}
	else if(whichSide == Direction::up)
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

Line Bounds::GetSide(Direction whichSide) const
{
	assert(validDirection(whichSide));

	Bounds retval;
	transfer_side(*this, retval, whichSide);

	// getting a side from a bounded rectangle
	// is "lossy", so we need to set mins = maxs
	// or maxs = mins to "lose" the data
	if(whichSide == Direction::left)
		retval.max.x = retval.min.x;
	else if(whichSide == Direction::right)
		retval.min.x = retval.max.x;
	else if(whichSide == Direction::up)
		retval.max.y = retval.min.y;
	else
		retval.min.y = retval.max.y;

	return retval;
}

void Bounds::SetSide(Line side, Direction whichSide)
{
	assert(validDirection(whichSide));
	transfer_side(side, *this, whichSide);
}
