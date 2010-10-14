#include "Color24.hpp"

#include <SDL_video.h>

Color24::Color24() :
	r(0), g(0), b(0)
{
}

Color24::Color24(unsigned short _r, unsigned short _g, unsigned short _b) :
	r(_r), g(_g), b(_b)
{
}

Uint32 Color24::GetRGBMap(SDL_Surface* surface) const
{
	return SDL_MapRGB(surface->format, red, green, blue);
}
