#pragma once

#include "Color24.hpp"
#include "WorldObject.hpp"

struct Point;

class Screen
{
private:
	SDL_Surface* surface;
	unsigned long width, height;
	Color24 bgColor;

public:
	Screen(unsigned long width, unsigned long height);
	~Screen();

	Point GetCenter() const;
	Point GetBounds() const;

	// get the SDL_Surface* representing the screen
	SDL_Surface* GetSurface() const;

	void Update() const;
	void Clear() const;
};
