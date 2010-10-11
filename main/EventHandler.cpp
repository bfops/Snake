#include "EventHandler.hpp"

#include "Common.hpp"
#include "GameState.hpp"

#include <SDL/SDL_events.h>

bool EventHandler::GetAndHandleEvent(GameState& gameState, ZippedUniqueObjectList& gameObjects)
{
	SDL_Event event;

	if(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				gameState.QuitHandler();
				break;

			case SDL_KEYDOWN:
			{
				const SDLKey key = event.key.keysym.sym;
				if(!gameState.IsPaused())
					gameState.KeyHandler(key, gameObjects);

				if(key == SDLK_p)
					gameState.PauseHandler();
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
				if(!gameState.IsPaused())
					gameState.MouseHandler(event.button.button, gameObjects);
				break;
		}

		return true;
	}

	return false;
}
