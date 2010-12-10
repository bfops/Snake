#include "Music.hpp"

#include "Logger.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/format.hpp>
#include <string>

#ifdef MSVC
#pragma warning(pop)
#endif

using namespace boost;

Music::Music()
{
	music = NULL;
}

Music::Music(const std::string& filename)
{
	music = Mix_LoadMUS(filename.c_str());
	if(music == NULL)
		Logger::Fatal(format("Error playing music \"%1%\": %2%") % filename.c_str() % Mix_GetError());
	Mix_PlayMusic(music, -1);
}

Music::~Music()
{
	Mix_FreeMusic(music);
}
