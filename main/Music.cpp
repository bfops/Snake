#include "Music.hpp"

#include "Logger.hpp"

#ifdef MSVC
#pragma warning( push, 0 )
#endif

#include <boost/format.hpp>

#ifdef MSVC
#pragma warning( pop )
#endif

using namespace boost;

static Logger::Handle logger(Logger::RequestHandle("Music"));

Music::Music(const char*const filename)
{
	music = Mix_LoadMUS(filename);
	if(music == NULL)
		logger.Fatal(format("Error playing music: %1%") % Mix_GetError());
	Mix_PlayMusic(music, -1);
}

Music::~Music()
{
	Mix_FreeMusic(music);
}
