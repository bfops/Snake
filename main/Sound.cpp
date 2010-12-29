#include "Sound.hpp"

#include "Config.hpp"
#include "Logger.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <string>
#include <SDL.h>

#ifdef MSVC
#pragma warning(pop)
#endif

Sound::Sound(const std::string& filename)
{
	if(Config::Get().sound)
	{
		sound = Mix_LoadWAV(filename.c_str());

		// SDL error conditions
		if(sound == NULL || (channel = Mix_PlayChannel(-1, sound, 0)) == -1)
		{
			Logger::Debug(boost::format("Error playing sound \"%1%\": %2%")
				% filename.c_str() % Mix_GetError());
			return;
		}
	}
	else
	{
		sound = NULL;
		channel = -1;
	}
}

void Sound::Stop() const
{
	if(sound)
		Mix_FreeChunk(sound);
	if(channel >= 0)
		Mix_HaltChannel(channel);
}

bool Sound::IsDone() const
{
	if(channel < 0)
		return true;

	return (Mix_Playing(channel) == 0);
}
