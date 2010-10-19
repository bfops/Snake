#pragma once

#include "SDL_mixer.h"

class Music
{
private:
	Mix_Music* music;

public:
	Music(const char* const filename);
	~Music();
};