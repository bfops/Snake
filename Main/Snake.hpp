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

	/// change the Snake's direction to that provided
	void ChangeDirection(Direction newDirection, ZippedUniqueObjectList& gameObjects);
	/// turn the snake relative to the direction provided
	void Turn(Direction turnDirection, ZippedUniqueObjectList& gameObjects);

	void Update(ZippedUniqueObjectList& gameObjects, unsigned int elapsedMilliseconds);
	bool IsDead() const;
};
