#include <SDL/SDL.h>

#include "Common.hpp"
#include "Logger.hpp"

namespace {
Logger::Handle logger = Logger::RequestHandle("SDLInitializer");

struct SDLInitializer
{
	SDLInitializer()
	{
		if(SDL_Init(SDL_INIT_VIDEO))
			logger.Fatal("SDL failed to initialize");
	}

	~SDLInitializer()
	{
		SDL_Quit();
	}
} init;
}
