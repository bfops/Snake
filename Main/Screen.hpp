#ifndef __Screen__
#define __Screen__

#include <SDL/SDL.h>

#include "Color24.hpp"
#include "Point.hpp"
#include "Wall.hpp"
#include "WorldObject.hpp"

class Screen
{
private:
	SDL_Surface* screen;

	const size_t width;
	const size_t height;
	const size_t blockWidth;

	Color24 bgColor;

	Wall walls[4];

public:
	const Point bottomRight;

	Screen(size_t width, size_t height, size_t horizontalBlocks, size_t verticalBlocks);

	SDL_Surface* GetSurface();

	Point ResolveIndex(const Point blockIndex) const;
	void Update();
	void Clear();
	void Draw(const WorldObject& object, const Color24& color);
};

#endif