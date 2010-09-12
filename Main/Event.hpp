#pragma once

#include <SDL/SDL_events.h>

#include "Snake.hpp"

namespace Event
{
	// URGENT TODO: figure out nicer way of handling events
	// than having to have registered objects
	int SDLCALL Handler(const SDL_Event*);
	void RegisterPlayer(Snake& player);
	void RegisterQuitter(bool& quitter);
}
