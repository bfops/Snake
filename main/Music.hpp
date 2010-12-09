#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <SDL_mixer.h>

#ifdef MSVC
#pragma warning(pop)
#endif

class Music
{
private:
	Mix_Music* music;

public:
	Music();
	Music(const char* const filename);
	~Music();
};
