#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <SDL_mixer.h>

#ifdef MSVC
#pragma warning(pop)
#endif

// whether or not music is on
#define MUSIC

class Music
{
private:
	Mix_Music* music;

public:
	Music(const char* const filename);
	~Music();
};