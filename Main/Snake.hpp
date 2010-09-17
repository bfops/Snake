#pragma once

#include <cstddef>

#include "cgq.hpp"
#include "Common.hpp"
#include "Direction.hpp"
#include "SnakeSegment.hpp"
#include "Timer.hpp"
#include "Vector2D.hpp"
#include "World.hpp"

// TODO: color snake head
class Snake
{
public:
	typedef cgq<SnakeSegment*> Path;

private:
	unsigned int length;
	unsigned int projectedLength;
	Path path;

	unsigned int speed;

	Timer moveTimer;
	Timer growTimer;
	Timer speedupTimer;

	void Grow(size_t amount);

	inline SnakeSegment& Head();
	inline SnakeSegment& Tail();

public:
	Snake(World& world);

	void Reset(World& world);
	void ChangeDirection(Direction, World& world);
	void Update();
	bool IsDead() const;
};
