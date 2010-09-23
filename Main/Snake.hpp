#pragma once

#include "cgq.hpp"
#include "Common.hpp"
#include "Direction.hpp"
#include "SnakeSegment.hpp"
#include "Timer.hpp"
#include "UniqueObjectList.hpp"

class GameWorld;
class Snake
{
public:
	typedef cgq<SnakeSegment> Path;

private:
	Logger::Handle logger;

	unsigned int length;
	unsigned int projectedLength;
	Path path;

	unsigned int speed;

	Timer moveTimer;
	Timer speedupTimer;

	void Grow(int amount);

	inline SnakeSegment& Head();
	inline SnakeSegment& Tail();

	void Init(Point centerOfScreen, UniqueObjectList& gameObjects);

public:
	Snake(Point centerOfScreen, UniqueObjectList& gameObjects);

	void Reset(Point centerOfScreen, UniqueObjectList& gameObjects);
	void ChangeDirection(Direction, UniqueObjectList& gameObjects);
	void Update(UniqueObjectList& gameObjects);
	bool IsDead() const;
};
