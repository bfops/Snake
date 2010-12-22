#include "Screen.hpp"

#include "Config.hpp"
#include "Logger.hpp"
#include "Point.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <SDL_video.h>

#ifdef MSVC
#pragma warning(pop)
#endif

Screen::Screen(const unsigned long _width, const unsigned long _height)
{
	width = _width;
	height = _height;
	surface = SDL_SetVideoMode(width, height, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
	bgColor = Config::Get().bgColor;

	if(surface == NULL)
		Logger::Fatal(boost::format("Error creating screen: %1%") % SDL_GetError());
}

Screen::~Screen()
{
	SDL_FreeSurface(surface);
}

SDL_Surface* Screen::GetSurface() const
{
	++surface->refcount;
	return surface;
}

Point Screen::GetCenter() const
{
	return Point(width / 2, height / 2);
}

Point Screen::GetBounds() const
{
	return Point(width, height);
}

void Screen::Update() const
{
	if(SDL_Flip(surface) != 0)
		Logger::Fatal(boost::format("Error updating screen: %1%") % SDL_GetError());
}

void Screen::Clear() const
{
	SDL_Rect blank;
	blank.x = blank.y = 0;
	blank.w = width;
	blank.h = height;

	SDL_FillRect(surface, &blank, bgColor.GetRGBMap(surface));
}
