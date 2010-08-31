#include <cassert>

#include "Screen.hpp"

Screen::Screen(size_t _width, size_t _height) :
	width(_width), height(_height), bgColor(0, 0, 0), blockWidth(15), bottomRight(width / blockWidth, height / blockWidth)
{
	// the blocks should fit evenly into the screen with no leftover space
	assert((width % blockWidth) == (height % blockWidth) && (width % blockWidth) == 0);
	// there should be a "center" block in both dimensions
	assert((bottomRight.x % 2) == (bottomRight.y % 2) && (bottomRight.x % 2) == 0);
	// TODO: check for errors in return value
	screen = SDL_SetVideoMode(width, height, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
}

template <typename T>
static T* fix_refcount(T* x)
{
	++x->refcount;
	return x;
}

const SDL_Surface* Screen::GetSurface() const
{
	return fix_refcount(screen);
}
SDL_Surface* Screen::GetSurface()
{
	return fix_refcount(screen);
}

Point Screen::ResolveIndex(Point p) const
{
	return Point(blockWidth * p.x, blockWidth * p.y);
}
void Screen::Update()
{
	// TODO: check return value
	SDL_Flip(screen);
}
void Screen::Clear()
{
	SDL_Rect blank;
	blank.x = blank.y = 0;
	blank.w = width;
	blank.h = height;
	SDL_FillRect(screen, &blank, SDL_MapRGB(screen->format, bgColor.red, bgColor.green, bgColor.blue));
}
