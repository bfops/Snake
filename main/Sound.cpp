#include "Sound.hpp"

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
	sound = Mix_LoadWAV(filename.c_str());

	if(sound == NULL || (channel = Mix_PlayChannel(-1, sound, 0)) == -1)
	{
		Logger::Debug(boost::format("Error playing sound \"%1%\": %2%") % filename.c_str() % Mix_GetError());
		return;
	}
}

Sound::Sound(Sound& obj)
{
	sound = obj.sound;
	channel = obj.channel;
	obj.sound = NULL;
	obj.channel = -1;
}

Sound::~Sound()
{
	if(sound)
		Mix_FreeChunk(sound);
}

void Sound::WaitForCompletion() const
{
	if(channel < 0)
		return;

	while(Mix_Playing(channel))
		SDL_Delay(1);
}
