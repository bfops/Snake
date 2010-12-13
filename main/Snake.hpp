#pragma once

#include "SnakeSegment.hpp"
#include "Timer.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/thread/recursive_mutex.hpp>
#include <list>

#ifdef MSVC
#pragma warning(pop)
#endif

class Direction;
class GameWorld;
class Food;
struct ZippedUniqueObjectList;

class Snake
{
public:
	typedef std::list<SnakeSegment> Path;

private:
	boost::recursive_mutex pathMutex;

	unsigned int length;
	unsigned int targetLength;
	// all the segments in the snake
	Path path;

	unsigned int speed;

	Timer moveTimer;
	Timer speedupTimer;
	Timer pointTimer;

	unsigned long long points;

	void AddSegment(Point location, Direction directionOfTravel, ZippedUniqueObjectList& gameObjects);
	void Grow(int amount);

	// return the first segment
	inline SnakeSegment& Head();
	// return the last segment
	inline SnakeSegment& Tail();

	void Init(Point centerOfScreen, ZippedUniqueObjectList& gameObjects);

public:
	Snake(Point centerOfScreen, ZippedUniqueObjectList& gameObjects);

	void Reset(Point centerOfScreen, ZippedUniqueObjectList& gameObjects);

	// remove the tail, since it's empty
	void RemoveEmptyTail(ZippedUniqueObjectList& gameObjects);

	// change the Snake's direction to that provided
	void ChangeDirection(Direction newDirection, ZippedUniqueObjectList& gameObjects);
	// turn the snake relative to the direction provided
	void Turn(Direction turnDirection, ZippedUniqueObjectList& gameObjects);

	void Update(ZippedUniqueObjectList& gameObjects);

	void EatFood(const Food&);
};
