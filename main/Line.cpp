#include "Line.hpp"

void Line::ApplyVector(Vector2D vector, unsigned int scale)
{
	min.x += vector.x * scale;
	min.y += vector.y * scale;
}
