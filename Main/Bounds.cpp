#include "Bounds.hpp"
#include "Common.hpp"
#include <boost/concept_check.hpp>

Bounds::Bounds()
{
}
Bounds::Bounds(Point _min, Point _max) :
	min(_min), max(_max)
{
}

namespace {
/// transfers side data from one locus to another
void transfer_side(const Bounds& input, Bounds& output, Direction side)
{
	if(side == Direction::left())
	{
		output.min = input.min;
		output.max.y = input.max.y;
	}
	else if(side == Direction::right())
	{
		output.min.y = input.min.y;
		output.max = input.max;
	}
	else if(side == Direction::up())
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
Bounds Bounds::GetSide(Direction side)
{
	Bounds retval;
	transfer_side(*this, retval, side);

	// getting a side is "lossy", so we
	// need to lose some data here by setting
	// mins = maxs and maxs = mins
	if(side == Direction::left())
		retval.max.x = min.x;
	else if(side == Direction::right())
		retval.min.x = max.x;
	else if(side == Direction::up())
		retval.max.y = min.y;
	else
		retval.min.y = max.y;

	return retval;
}
void Bounds::SetSide(Bounds bounds, Direction side)
{
	transfer_side(bounds, *this, side);
}
