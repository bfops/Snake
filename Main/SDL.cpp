#include "SDL.h"
#include <SDL/SDL.h>
#include <stdexcept>

SDLInitializer::SDLInitializer()
{
	if(SDL_Init(SDL_INIT_VIDEO))
		throw std::runtime_error("SDL failed to initialize");
}

SDLInitializer::~SDLInitializer()
{
	SDL_Quit();
}