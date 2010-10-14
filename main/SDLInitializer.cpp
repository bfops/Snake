#include "SDLInitializer.hpp"

#include "Logger.hpp"

#include <SDL.h>
#include <SDL_mixer.h>

static Logger::Handle logger(Logger::RequestHandle("SDL"));
SDLInitializer::SDLInitializer()
{
	if(SDL_Init(SDL_INIT_VIDEO))
		logger.Fatal("SDL failed to initialize");


}

SDLInitializer::~SDLInitializer()
{
	SDL_Quit();
}
