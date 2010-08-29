#ifndef __Screen__
#define __Screen__

#include <SDL/SDL.h>

#include "Point.hpp"

class Screen
{
private:
	SDL_Surface* screen;

	const size_t width;
	const size_t height;
public:
	static const size_t blockWidth = 20;

	Screen(size_t width, size_t height);

	SDL_Surface* GetSurface();
	const SDL_Surface* GetSurface() const;

	Point ResolveIndex(const Point p) const;
	void Update();
};

#endif