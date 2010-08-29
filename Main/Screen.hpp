#ifndef __Screen__
#define __Screen__

#include <SDL/SDL.h>

#include "Color24.hpp"
#include "Point.hpp"

class Screen
{
private:
	SDL_Surface* screen;

	const size_t width;
	const size_t height;

	Color24 bgColor;

public:
	const size_t blockWidth;
	const Point bottomRight;

	Screen(size_t width, size_t height);

	SDL_Surface* GetSurface();
	const SDL_Surface* GetSurface() const;

	Point ResolveIndex(const Point p) const;
	void Update();
	void Clear();
};

#endif