#pragma once

#include <SDL/SDL.h>

#include "Color24.hpp"
#include "Point.hpp"
#include "Wall.hpp"
#include "WorldObject.hpp"

class Screen
{
private:
	SDL_Surface* screen;

	const unsigned int width, height;
	const unsigned int xBlocks, yBlocks;
	const unsigned int blockWidth;

	Color24 bgColor;

public:
	Screen(unsigned int width, unsigned int height, unsigned int horizontalBlocks, unsigned int verticalBlocks);

	SDL_Surface* GetSurface();
	Point GetCenter() const;
	Point GetBlockBounds() const;

	Point ResolveIndex(const Point blockIndex) const;
	void Update();
	void Clear();
	void Draw(const WorldObject& object, const Color24& color);
};
