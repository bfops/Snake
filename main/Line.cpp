#include "Line.hpp"

void Line::ApplyVector(const Vector2D vector, const unsigned long scale)
{
	min.x += vector.x * scale;
	min.y += vector.y * scale;
}
