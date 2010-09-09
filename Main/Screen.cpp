#include <cassert>
#include <stdexcept>
#include <string>

#include "Common.hpp"
#include "Physics.hpp"
#include "Point.hpp"
#include "Screen.hpp"
#include "Wall.hpp"

using namespace std;

Screen::Screen(unsigned int _width, unsigned int _height) :
	width(_width), height(_height), bgColor(0, 0, 0)
{
	screen = SDL_SetVideoMode(width, height, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
	if(screen == NULL)
	{
		string error = "Error creating screen: ";
		error += SDL_GetError();
		throw runtime_error(error.c_str());
	}
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
	if(SDL_Flip(screen) == -1)
	{
		string error = "Error updating screen: ";
		error += SDL_GetError();
		throw runtime_error(error.c_str());
	}
}
void Screen::Clear()
{
	SDL_Rect blank;
	blank.x = blank.y = 0;
	blank.w = width;
	blank.h = height;
	SDL_FillRect(screen, &blank, bgColor.GetRGBMap(screen));
}
