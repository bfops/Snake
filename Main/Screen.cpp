#include "Screen.hpp"

#include "Common.hpp"
#include "Logger.hpp"

using namespace std;

Screen::Screen(unsigned int _width, unsigned int _height) :
	logger(Logger::RequestHandle("Screen")), width(_width), height(_height), bgColor(0, 0, 0)
{
	if((screen = SDL_SetVideoMode(width, height, 0, SDL_ANYFORMAT | SDL_SWSURFACE)) == NULL)
		logger.Fatal(boost::format("Error creating screen: %1%") % SDL_GetError());
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
		logger.Fatal(boost::format("Error updating screen: %1%") % SDL_GetError());
}
void Screen::Clear()
{
	SDL_Rect blank;
	blank.x = blank.y = 0;
	blank.w = width;
	blank.h = height;
	SDL_FillRect(screen, &blank, bgColor.GetRGBMap(screen));
}
