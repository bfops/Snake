#ifndef __Snake__
#define __Snake__

#include <deque>
#include <SDL/SDL.h>

#include "Common.hpp"
#include "Point.hpp"
#include "Screen.hpp"
#include "SnakeSegment.hpp"
#include "Timer.hpp"

class Snake
{
private:
	enum Direction
	{
		left,
		right,
		up,
		down
	};
	const static Direction directions[];
	typedef std::vector<SnakeSegment> Path;

	unsigned int length;
	Direction direction;
	Path path;
	Path::iterator head;
	Screen* screen;

	Timer moveTimer;
	Timer growTimer;

public:
	Snake();

	void Reset();
	void GetInput();
	void Update();
	void SetRenderTarget(Screen& target);
	void Center();
	void Draw() const;
	bool IsDead() const;
};

#endif