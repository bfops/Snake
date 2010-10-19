#include "EventHandler.hpp"

#include <SDL_events.h>

EventHandler::EventHandler(QuitCallbackType onquit, LossCallbackType onloss, PauseCallbackType onpause,
	KeyCallbackType onkey, MouseCallbackType onmouse) :
	quitCallback(onquit), lossCallback(onloss), pauseCallback(onpause), keyCallback(onkey),
	mouseCallback(onmouse)
{
}

void EventHandler::HandleEventQueue(ZippedUniqueObjectList& gameObjects) const
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

void EventHandler::LossNotify() const
{
	lossCallback();
}

const EventHandler*& EventHandler::GetCurrentEventHandler()
{
	static const EventHandler* eventHandler;
	return eventHandler;
}
