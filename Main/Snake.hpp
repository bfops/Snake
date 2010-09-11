#pragma once

#include <vector>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "Logger.hpp"
#include "SnakeSegment.hpp"
#include "Timer.hpp"
#include "Vector2D.hpp"

#include <cstddef>

// URGENT TODO: store snakes as vectors of direction-length pairs
class Snake
{
public:
	// TODO: make Direction::Direction private so that only
	// certain directions make exist
	typedef Vector2D Direction;
	const static Direction right, left, up, down;

private:
	typedef std::vector<SnakeSegment> Path;

	Logger::Handle logger;

	Direction direction;
	unsigned int length;
	Path path;

	unsigned int speed;

	Timer moveTimer;
	Timer growTimer;
	Timer speedupTimer;

	void AddTailSegment(Point location);
	void Grow(size_t amount = 1);

public:
	Snake(Point headLocation);

	void Reset(Point headLocation);
	void ChangeDirection(Direction);
	void Update();
	bool IsDead() const;
};
