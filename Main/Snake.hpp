#ifndef __Snake__
#define __Snake__

#include <vector>
#include <SDL/SDL.h>

#include "Color24.hpp"
#include "Common.hpp"
#include "Point.hpp"
#include "Screen.hpp"
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
	typedef std::vector<Point> Path;

	unsigned int length;
	Direction direction;
	Path path;
	Path::iterator head;
	Color24 color;
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