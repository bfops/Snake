#include "EventHandler.hpp"

#include "Common.hpp"
#include "GameState.hpp"

#include <SDL/SDL_events.h>

void EventHandler::Update(GameState& gameState, ZippedUniqueObjectList& gameObjects)
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
				if(key == SDLK_p)
					gameState.PauseHandler();

				gameState.KeyHandler(key, gameObjects);
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
				gameState.MouseHandler(event.button.button, gameObjects);
				break;
		}
	}
}
