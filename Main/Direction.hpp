#pragma once

#include "Vector2D.hpp"

class Direction
{
private:
	Direction(int x, int y);
	Vector2D direction;

public:
	// TODO: make inline?
	static Direction empty();
	static Direction up();
	static Direction down();
	static Direction left();
	static Direction right();

	operator Vector2D&();
	Direction operator-() const;
	bool operator==(const Direction&) const;
	bool operator!=(const Direction&) const;
};