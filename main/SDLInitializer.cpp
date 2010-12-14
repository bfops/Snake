#include "SDLInitializer.hpp"

#include "Logger.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <SDL.h>
#include <SDL_mixer.h>

#ifdef MSVC
#pragma warning(pop)
#endif

SDLInitializer::SDLInitializer()
{
	if(SDL_Init(SDL_INIT_VIDEO))
		Logger::Fatal(boost::format("Error initializing SDL: %1%") % SDL_GetError());

	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 4096))
		Logger::Fatal(boost::format("Error initializing SDL_Mixer: %1%") % Mix_GetError());
}

SDLInitializer::~SDLInitializer()
{
	Mix_CloseAudio();
	SDL_Quit();
}
