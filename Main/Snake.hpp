#pragma once

#include <deque>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "Screen.hpp"
#include "SnakeSegment.hpp"
#include "Timer.hpp"
#include "Vector2D.hpp"

// TODO: make death semantics prettier
class Snake
{
public:
	typedef Vector2D Direction;
	const static Direction right, left, up, down;

private:
	typedef std::vector<SnakeSegment> Path;

	Color24 headColor, bodyColor;

	unsigned int length;
	Direction direction;
	Path path;

	Timer moveTimer;
	Timer growTimer;

	void AddTailSegment(Point location);

public:
	Snake(Point headLocation);

	void Reset(Point headLocation);
	void ChangeDirection(Direction);
	void Update();
	void Draw(Screen& target) const;
	bool IsDead() const;
};
