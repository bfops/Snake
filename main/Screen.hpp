#pragma once

#include "Color24.hpp"

struct Point;

class Screen
{
private:
	SDL_Surface* screen;

	unsigned long width, height;
	Color24 bgColor;

public:
	Screen(unsigned long width, unsigned long height);
	~Screen();

	SDL_Surface* GetSurface();
	Point GetCenter() const;
	Point GetBounds() const;

	void Update();
	void Clear();
};
