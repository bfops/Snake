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
private:
	typedef Uint8 ColorType;

public:
	union
	{
		ColorType r;
		ColorType red;
	};
	union
	{
		ColorType g;
		ColorType green;
	};
	union
	{
		ColorType b;
		ColorType blue;
	};

	Color24();
	Color24(ColorType red, ColorType green, ColorType blue);

	// convert to an SDL RGBMap corresponding to _surface_
	Uint32 GetRGBMap(const SDL_Surface* _surface_) const;
};
