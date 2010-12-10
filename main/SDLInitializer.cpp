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
		Logger::Fatal("SDL failed to initialize");

	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 4096))
		Logger::Fatal("SDL_Mixer failed to initialize");
}

SDLInitializer::~SDLInitializer()
{
	Mix_CloseAudio();
	SDL_Quit();
}
