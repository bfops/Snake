#pragma once

#include <SDL/SDL.h>

#include "Color24.hpp"
#include "Common.hpp"
#include "Logger.hpp"
#include "Point.hpp"

class Screen
{
private:
	Logger::Handle logger;

	SDL_Surface* screen;

	const unsigned int width, height;
	Color24 bgColor;

public:
	static DEF_CONSTANT(Color24, transparent, Color24(255, 0, 255))

	Screen(unsigned int width, unsigned int height);

	SDL_Surface* GetSurface();
	Point GetCenter() const;
	Point GetBounds() const;

	void Update();
	void Clear();
};
