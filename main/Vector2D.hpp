#pragma once

// 2D vector class
struct Vector2D
{
	long x, y;

	Vector2D();
	Vector2D(long x, long y);

	Vector2D& operator+=(Vector2D);
	Vector2D operator-(Vector2D) const;
	Vector2D operator-() const;
	Vector2D operator*(long scale) const;

	bool operator==(Vector2D) const;
	bool operator!=(Vector2D) const;
};
