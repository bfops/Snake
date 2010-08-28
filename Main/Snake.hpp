#ifndef __SNAKE__
#define __SNAKE__

#include <vector>
#include <SDL/SDL.h>

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

public:
	void GetInput();
	void Update();
	void Draw(SDL_Surface* target) const;
	bool IsDead() const;
};

#endif