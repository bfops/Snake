#pragma once

#include <cstddef>

#include "cgq.hpp"
#include "Common.hpp"
#include "Direction.hpp"
#include "Logger.hpp"
#include "SnakeSegment.hpp"
#include "Timer.hpp"
#include "Vector2D.hpp"

// TODO: modify so that when SnakeSegments die
// or eat, the Snake is notified immediately
// TODO: color snake head
class Snake
{
private:
	typedef cgq<SnakeSegment> Path;

	unsigned int length;
	Path path;

	unsigned int speed;

	Timer moveTimer;
	Timer growTimer;
	Timer speedupTimer;

	void AddTailSegment(Point location, Direction);
	void Grow(size_t amount);

	inline SnakeSegment& Head();
	inline SnakeSegment& Tail();

public:
	Snake(Point headLocation);

	void Reset(Point headLocation);
	void ChangeDirection(Direction);
	void Update();
	bool IsDead() const;
};
