#pragma once

#ifdef MSVC
#pragma warning( push, 0 )
#endif

#include <SDL_types.h>

#ifdef MSVC
#pragma warning( pop )
#endif

struct SDL_Surface;

struct Color24
{
private:
	typedef unsigned short ColorType;

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

	Uint32 GetRGBMap(SDL_Surface*) const;
};
