#include "Vector2D.hpp"

Vector2D::Vector2D() :
	x(0), y(0)
{
}

Vector2D::Vector2D(const long _x, const long _y)
{
	x = _x;
	y = _y;
}

Vector2D Vector2D::operator-() const
{
	return Vector2D(-x, -y);
}

bool Vector2D::operator==(const Vector2D& obj) const
{
	return ((x == obj.x) && (y == obj.y));
}

bool Vector2D::operator!=(const Vector2D& obj) const
{
	return !(*this == obj);
}
