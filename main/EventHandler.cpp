#include "EventHandler.hpp"

#include "GameState.hpp"

#include <SDL_events.h>

EventHandler::EventHandler(QuitCallbackType onquit, PauseCallbackType onpause,
	KeyCallbackType onkey, MouseCallbackType onmouse) :
	quitCallback(onquit), pauseCallback(onpause), keyCallback(onkey), mouseCallback(onmouse)
{
}

void EventHandler::HandleEventQueue(GameState& gameState, ZippedUniqueObjectList& gameObjects) const
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				if(quitCallback)
					quitCallback();
				break;

			case SDL_KEYDOWN:
			{
				const SDLKey key = event.key.keysym.sym;

				if(key == SDLK_p)
				{
					if(pauseCallback)
						pauseCallback();
				}
				else if(keyCallback)
					keyCallback(key);

				break;
			}

			case SDL_MOUSEBUTTONDOWN:
				if(mouseCallback)
					mouseCallback(event.button.button);
				break;
		}
	}
}
