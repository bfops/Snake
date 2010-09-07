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

	Color24 color;
	Screen* screen;

	static const unsigned int defaultLength = 3;
	unsigned int length;
	Direction direction;
	Path path;

	Timer moveTimer;
	Timer growTimer;

	bool dead;

	void AddSegment();

public:
	Snake();

	void Reset();
	void ChangeDirection(Direction);
	void Update();
	void SetRenderTarget(Screen& target);
	void Center();
	void Draw() const;
	bool IsDead() const;
};
