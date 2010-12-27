#include "Line.hpp"

Line Line::operator+(const Vector2D v) const
{
	Line newLine(*this);
	newLine.min += v;

	return newLine;
}

Line& Line::operator+=(const Vector2D v)
{
	min += v;
	return *this;
}
