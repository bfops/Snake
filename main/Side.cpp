#include "Side.hpp"

void Side::ApplyVector(Vector2D vector, unsigned int scale)
{
	min.x += vector.x * scale;
	min.y += vector.y * scale;
}
