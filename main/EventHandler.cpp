#include "EventHandler.hpp"

#include "GameState.hpp"

#include <SDL_events.h>

void EventHandler::HandleEventQueue(GameState& gameState, ZippedUniqueObjectList& gameObjects)
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				quitCallback();
				break;

			case SDL_KEYDOWN:
			{
				const SDLKey key = event.key.keysym.sym;

				if(key == SDLK_p)
					pauseCallback();
				else
					keyCallback(key);

				break;
			}

			case SDL_MOUSEBUTTONDOWN:
				mouseCallback(event.button.button);
				break;
		}
	}
}
