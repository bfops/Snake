#pragma once

#include "cgq.hpp"
#include "Common.hpp"
#include "Direction.hpp"
#include "SnakeSegment.hpp"
#include "Timer.hpp"
#include "ObjectManager.hpp"

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

	void Grow(int amount);

	inline SnakeSegment& Head();
	inline SnakeSegment& Tail();

public:
	Snake(ObjectManager& world);

	void Reset(ObjectManager& world);
	void ChangeDirection(Direction, ObjectManager& world);
	void Update(ObjectManager& world);
	bool IsDead() const;
};
