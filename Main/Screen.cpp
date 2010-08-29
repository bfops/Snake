#include "Screen.hpp"

Screen::Screen(size_t _width, size_t _height) :
	width(_width), height(_height)
{
	// TODO: check for errors in return value
	screen = SDL_SetVideoMode(width, height, 0, SDL_ANYFORMAT | SDL_SWSURFACE | SDL_DOUBLEBUF);
}

const SDL_Surface* Screen::GetSurface() const
{
	++screen->refcount;
	return screen;
}
SDL_Surface* Screen::GetSurface()
{
	return const_cast<SDL_Surface*>(const_cast<const Screen*>(this)->GetSurface());
}

Point Screen::ResolveIndex(Point p) const
{
	return Point(blockWidth * p.x, blockWidth * p.y);
}
void Screen::Update()
{
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}
