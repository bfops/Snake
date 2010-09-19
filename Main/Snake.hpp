#pragma once

#include "cgq.hpp"
#include "Common.hpp"
#include "Direction.hpp"
#include "SnakeSegment.hpp"
#include "Timer.hpp"

// TODO: shitshitshit circular dependencies!
class GameWorld;
// TODO: color snake head
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
	Timer growTimer;
	Timer speedupTimer;

	void Grow(int amount);

	inline SnakeSegment& Head();
	inline SnakeSegment& Tail();

public:
	Snake(GameWorld& world);

	void Reset(GameWorld& world);
	void ChangeDirection(Direction, GameWorld& world);
	void Update(GameWorld& world);
	bool IsDead() const;
};
