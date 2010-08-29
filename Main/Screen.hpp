#ifndef __Screen__
#define __Screen__

#include <SDL/SDL.h>

class Screen
{
private:
	SDL_Surface* screen;

	const size_t width;
	const size_t height;
public:
	static const size_t blockSize = 50;

	Screen(size_t width, size_t height);

	operator SDL_Surface*();
};

#endif