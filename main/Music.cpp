#include "Music.hpp"

#include "Logger.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <string>

#ifdef MSVC
#pragma warning(pop)
#endif

Music::Music()
{
	music = NULL;
}

Music::Music(const std::string& filename)
{
	music = Mix_LoadMUS(filename.c_str());

	if(music == NULL)
		Logger::Debug(boost::format("Error playing music \"%1%\": %2%") %
			filename.c_str() % Mix_GetError());
	else
		Mix_PlayMusic(music, -1);
}

Music::Music(Music& obj)
{
	music = obj.music;
	obj.music = NULL;
}

Music::~Music()
{
	if(music)
		Mix_FreeMusic(music);
}
