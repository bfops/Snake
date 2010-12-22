#pragma once

// 2D vector class
struct Vector2D
{
	long x, y;

	Vector2D();
	Vector2D(long x, long y);

	// negation operator
	Vector2D operator-() const;

	bool operator==(const Vector2D&) const;
	bool operator!=(const Vector2D&) const;
};
