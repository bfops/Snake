#include <cassert>

#include "Common.hpp"
#include "Physics.hpp"
#include "Point.hpp"
#include "Screen.hpp"
#include "Wall.hpp"

Screen::Screen(unsigned int _width, unsigned int _height) :
	width(_width), height(_height), bgColor(0, 0, 0)
{
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
	return Point(width / 2, height / 2);
}
Point Screen::GetBounds() const
{
	return Point(width, height);
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
	rect.w = obj.width;
	rect.h = obj.height;
	rect.x = obj.location.x;
	rect.y = obj.location.y;

	// TODO: check for errors here
	SDL_FillRect(screen, &rect, color.GetRGBMap(screen));
}
