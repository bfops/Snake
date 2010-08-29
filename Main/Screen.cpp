#include "Screen.hpp"

Screen::Screen(size_t _width, size_t _height) :
	width(_width), height(_height)
{
	// TODO: check for errors in return value
	screen = SDL_SetVideoMode(width, height, 0, SDL_ANYFORMAT | SDL_SWSURFACE | SDL_DOUBLEBUF);
}
Screen::operator SDL_Surface*()
{
	return screen;
}
