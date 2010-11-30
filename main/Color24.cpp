#include "Color24.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <SDL_video.h>

#ifdef MSVC
#pragma warning(pop)
#endif

Color24::Color24() :
	r(0), g(0), b(0)
{
}

Color24::Color24(ColorType _r, ColorType _g, ColorType _b) :
	r(_r), g(_g), b(_b)
{
}

Uint32 Color24::GetRGBMap(SDL_Surface* surface) const
{
	return SDL_MapRGB(surface->format, red, green, blue);
}
