#ifndef __SNAKE__
#define __SNAKE__

#include <SDL/SDL.h>

class Snake
{
public:
	void GetInput();
	void Update();
	void Draw(SDL_Surface* target) const;
	bool IsDead() const;
};

#endif