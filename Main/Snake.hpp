#pragma once

#include "cgq.hpp"
#include "SnakeSegment.hpp"
#include "Timer.hpp"

class Direction;
class GameWorld;
class ZippedUniqueObjectList;

class Snake
{
public:
	typedef cgq<SnakeSegment> Path;

private:
	unsigned int length;
	unsigned int projectedLength;
	Path path;

	unsigned int speed;

	Timer moveTimer;
	Timer speedupTimer;

	void Grow(int amount);

	inline SnakeSegment& Head();
	inline SnakeSegment& Tail();

	void Init(Point centerOfScreen, ZippedUniqueObjectList& gameObjects);

public:
	Snake(Point centerOfScreen, ZippedUniqueObjectList& gameObjects);

	void Reset(Point centerOfScreen, ZippedUniqueObjectList& gameObjects);
	void ChangeDirection(Direction, ZippedUniqueObjectList& gameObjects);
	void Update(ZippedUniqueObjectList& gameObjects);
	bool IsDead() const;
};
