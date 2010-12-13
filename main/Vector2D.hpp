#pragma once

// 2D vector class
struct Vector2D
{
	int x, y;

	Vector2D();
	Vector2D(int x, int y);

	// negation operator
	Vector2D operator-() const;
	bool operator==(const Vector2D&) const;
	bool operator!=(const Vector2D&) const;
};
