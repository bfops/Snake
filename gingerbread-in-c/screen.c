#include "screen.h"

#include <assert.h>

static const int bitsperpixel = 32;

SDL_Surface* make_window(const char* title, Dimensions dims, bool fullScreen)
{
	SDL_WM_SetCaption(title, title);
	SDL_ShowCursor(SDL_DISABLE);

	unsigned flags =
		  SDL_ANYFORMAT
		| SDL_HWSURFACE | SDL_OPENGL | SDL_DOUBLEBUF
		| (fullScreen ? SDL_FULLSCREEN : 0)
	;

	SDL_Surface* screen = SDL_SetVideoMode(dims.width, dims.height, bitsperpixel, flags);

	assert(screen);
	return screen;
}
