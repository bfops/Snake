#ifndef __Snake__
#define __Snake__

#include <vector>
#include <SDL/SDL.h>

#include "Point.hpp"
#include "Screen.hpp"

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
	unsigned int length;
	std::vector<Direction> path;
	Point coordinate;

public:
	Snake();

	void Reset();
	void GetInput();
	void Update();
	void Draw(Screen& target) const;
	bool IsDead() const;
};

#endif