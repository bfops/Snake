#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <SDL_mixer.h>
#include <string>

#ifdef MSVC
#pragma warning(pop)
#endif

class Sound
{
private:
	Mix_Chunk* sound;
	int channel;

public:
	Sound(const std::string& filename);
	Sound(Sound&);
	~Sound();

	void WaitForCompletion() const;
};
