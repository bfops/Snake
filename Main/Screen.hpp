#pragma once

#include <SDL/SDL.h>

#include "Color24.hpp"
#include "Point.hpp"

// TODO: move screen into GraphicsWorld
class Screen
{
private:
	SDL_Surface* screen;

	const unsigned int width, height;

	Color24 bgColor;

public:
	Screen(unsigned int width, unsigned int height);

	SDL_Surface* GetSurface();
	Point GetCenter() const;
	Point GetBounds() const;

	void Update();
	void Clear();
};
