#include "SDL.h"

#include "Logger.hpp"

#include <SDL/SDL.h>

namespace {
Logger::Handle logger = Logger::RequestHandle("SDL");
}

SDLInitializer::SDLInitializer()
{
	if(SDL_Init(SDL_INIT_VIDEO))
		logger.Fatal("SDL failed to initialize");
}

SDLInitializer::~SDLInitializer()
{
	SDL_Quit();
}
