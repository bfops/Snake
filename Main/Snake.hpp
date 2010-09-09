#pragma once

#include <deque>
#include <vector>

#include <SDL/SDL.h>

#include "Common.hpp"
#include "Screen.hpp"
#include "SnakeSegment.hpp"
#include "Timer.hpp"
#include "Vector2D.hpp"

class Snake
{
public:
	typedef Vector2D Direction;
	const static Direction right, left, up, down;

private:
	typedef std::vector<SnakeSegment> Path;

	Color24 headColor, bodyColor;

	Direction direction;
	unsigned int length;
	Path path;

	unsigned int speed;

	Timer moveTimer;
	Timer growTimer;
	Timer speedupTimer;

	void AddTailSegment(Point location);
	void Grow();

public:
	Snake(Point headLocation);

	void Reset(Point headLocation);
	void ChangeDirection(Direction);
	void Update();
	void Draw(Screen& target) const;
	bool IsDead() const;
};
