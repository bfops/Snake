#include "SDL.h"

#include "Logger.hpp"

#include <SDL/SDL.h>

SDLInitializer::SDLInitializer() :
	logger(Logger::RequestHandle("SDL"))
{
	if(SDL_Init(SDL_INIT_VIDEO))
		logger.Fatal("SDL failed to initialize");
}

SDLInitializer::~SDLInitializer()
{
	SDL_Quit();
}
