#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <SDL_types.h>
#include <SDL_video.h>

#ifdef MSVC
#pragma warning(pop)
#endif

// 24-bit color
struct Color24
{
	typedef unsigned short ColorType;

	ColorType r, g, b;

	Color24();
	Color24(ColorType red, ColorType green, ColorType blue);

	// convert to an SDL RGBMap corresponding to _surface_
	Uint32 GetRGBMap(const SDL_Surface* _surface_) const;
};
