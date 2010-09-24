#pragma once

#include "cgq.hpp"
#include "SnakeSegment.hpp"
#include "Timer.hpp"

class Direction;
class GameWorld;
class UniqueObjectList;

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

	void Init(Point centerOfScreen, UniqueObjectList& graphicsObjects, UniqueObjectList& physicsObjects);

public:
	Snake(Point centerOfScreen, UniqueObjectList& graphicsObjects, UniqueObjectList& physicsObjects);

	void Reset(Point centerOfScreen, UniqueObjectList& graphicsObjects, UniqueObjectList& physicsObjects);
	void ChangeDirection(Direction, UniqueObjectList& graphicsObjects, UniqueObjectList& physicsObjects);
	void Update(UniqueObjectList& graphicsObjects, UniqueObjectList& physicsObjects);
	bool IsDead() const;
};
