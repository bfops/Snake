#pragma once

#include "Mutex.hpp"
#include "SnakeSegment.hpp"
#include "Timer.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

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
	RecursiveMutex pathMutex;
	Mutex attribMutex;

	unsigned long length;
	unsigned long targetLength;
	// all the segments in the snake
	Path path;

	unsigned short speed;

	Timer moveTimer;
	Timer speedupTimer;
	Timer pointTimer;

	// player points
	unsigned long long points;
	
	void AddSegment(ZippedUniqueObjectList& gameObjects);
	void AddHead(Point location, Direction directionOfTravel, ZippedUniqueObjectList& gameObjects);
	
	// return the segment to elongate
	SnakeSegment& Growable();
	// return the segment to shorten
	SnakeSegment& Shrinkable();
	// return the first segment
	SnakeSegment& Head();
	// return the last segment
	SnakeSegment& Tail();

	void Init(ZippedUniqueObjectList& gameObjects);

public:
	Snake(ZippedUniqueObjectList& gameObjects);

	void Reset(ZippedUniqueObjectList& gameObjects);

	void RemoveTail(ZippedUniqueObjectList& gameObjects);

	// change the Snake's direction to that provided
	void ChangeDirection(Direction newDirection, ZippedUniqueObjectList& gameObjects);
	// turn the snake relative to the direction provided
	void Turn(Direction turnDirection, ZippedUniqueObjectList& gameObjects);

	void Update(ZippedUniqueObjectList& gameObjects);

	void EatFood(const Food&);
};
