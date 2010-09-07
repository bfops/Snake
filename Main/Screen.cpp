#include <cassert>

#include "Common.hpp"
#include "Physics.hpp"
#include "Point.hpp"
#include "Screen.hpp"
#include "Wall.hpp"

Screen::Screen(unsigned int _width, unsigned int _height, unsigned int _xBlocks, unsigned int _yBlocks) :
	width(_width), height(_height), xBlocks(_xBlocks), yBlocks(_yBlocks), blockWidth(width / xBlocks),
	bgColor(0, 0, 0)
{
	// TODO: change so that black bars are used as buffers
	// if blocks don't fit. Block size is determined by
	// MIN(width / xBlocks, height / yBlocks)

	// the blocks should fit evenly into the screen with no leftover space
	assert((width % xBlocks) == 0 && (height % yBlocks) == 0);
	// there should be a "center" block in both dimensions
	assert((xBlocks % 2) == 0 && (yBlocks % 2) == 0);
	// blocks should be square, not rectangular
	assert(blockWidth == (height / yBlocks));

	// TODO: check for errors in return value
	screen = SDL_SetVideoMode(width, height, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
}

SDL_Surface* Screen::GetSurface()
{
	++(screen->refcount);
	return screen;
}
Point Screen::GetCenter() const
{
	return Point(xBlocks / 2, yBlocks / 2);
}
Point Screen::GetBlockBounds() const
{
	return Point(xBlocks, yBlocks);
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
	SDL_FillRect(screen, &blank, bgColor.GetRGBMap(screen));
}
void Screen::Draw(const WorldObject& obj, const Color24& color)
{
	SDL_Rect rect;
	rect.w = obj.width * blockWidth;
	rect.h = obj.height * blockWidth;
	Point realIndex = ResolveIndex(obj.location);
	rect.x = realIndex.x;
	rect.y = realIndex.y;

	// TODO: check for errors here
	SDL_FillRect(screen, &rect, color.GetRGBMap(screen));
}
