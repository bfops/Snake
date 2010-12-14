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

Bounds::Bounds(Point _min, Point _max)
{
	min = _min;
	max = _max;
}

Bounds::Bounds(Line side)
{
	min = max = side.min;

	if(side.horizontal)
		max.x += side.length;
	else
		max.y += side.length;
}

Bounds::operator Line() const
{
	// to be a line, one dimension must be 0
	assert(min.x == max.x || min.y == max.y);

	Line retval;
	retval.min = min;

#define DELTA(m) (max.m - min.m)
	if(min.x == max.x)
	{
		retval.length = DELTA(y);
		retval.horizontal = false;
	}
	else
	{
		retval.length = DELTA(x);
		retval.horizontal = true;
	}
#undef DELTA

	return retval;
}

// set _output_'s _whichSide_ side to equal _input_'s _whichSide_ side
static void transfer_side(const Bounds& input, Bounds& output, const Direction whichSide)
{
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

Line Bounds::GetSide(const Direction whichSide) const
{
	Bounds retval;
	transfer_side(*this, retval, whichSide);

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

void Bounds::SetSide(const Line side, const Direction whichSide)
{
	transfer_side(side, *this, whichSide);
}
