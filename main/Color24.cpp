#include "Color24.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <SDL_video.h>

#ifdef MSVC
#pragma warning(pop)
#endif

Color24::Color24()
{
	r = g = b = 0;
}

Color24::Color24(const ColorType _r, const ColorType _g, const ColorType _b)
{
	r = _r;
	g = _g;
	b = _b;
}

Uint32 Color24::GetRGBMap(const SDL_Surface* const surface) const
{
	return SDL_MapRGB(surface->format, red, green, blue);
}
