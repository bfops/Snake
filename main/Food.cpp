#include "Food.hpp"

Food::Food(const Point location, const unsigned short size, const Color24 color, const long long points,
	const double length, const short speed) :
	Spawn(food, location, size, color)
{
	pointChange = points;
	lengthFactor = length;
	speedChange = speed;
}

void Food::CollisionHandler(WorldObject& obj) const
{
	obj.CollisionHandler(*this);
}

long long Food::GetPointChange() const
{
	return pointChange;
}

double Food::GetLengthFactor() const
{
	return lengthFactor;
}

short Food::GetSpeedChange() const
{
	return speedChange;
}
