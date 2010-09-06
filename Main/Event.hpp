#pragma once

#include <SDL/SDL_events.h>

#include "Snake.hpp"

namespace Event
{
	int SDLCALL Handler(const SDL_Event*);
	void RegisterPlayer(Snake& player);
	void RegisterQuitter(bool& quitter);
}
