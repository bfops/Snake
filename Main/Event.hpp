#ifndef __Event__
#define __Event__

#include <SDL/SDL_events.h>

#include "Snake.hpp"

namespace Event
{
	int SDLCALL Handler(const SDL_Event*);
	void RegisterPlayer(Snake& player);
	void RegisterQuitter(bool& quitter);
}

#endif