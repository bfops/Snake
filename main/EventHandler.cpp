#include "EventHandler.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <SDL_events.h>

#ifdef MSVC
#pragma warning(pop)
#endif

boost::recursive_mutex EventHandler::mutex;

static EventHandler* eventHandler;

EventHandler::EventHandler(QuitCallbackType onquit, LossCallbackType onloss, PauseCallbackType onpause,
	KeyCallbackType onkey, MouseCallbackType onmouse)
{
	QuitCallback = onquit;
	LossCallback = onloss;
	PauseCallback = onpause;
	KeyCallback = onkey;
	MouseCallback = onmouse;
}

void EventHandler::HandleEventQueue() const
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				QuitCallback();
				break;

			case SDL_KEYDOWN:
			{
				const SDLKey key = event.key.keysym.sym;

				if(key == SDLK_p)
					PauseCallback();
				else
					KeyCallback(key);

				break;
			}

			case SDL_MOUSEBUTTONDOWN:
				MouseCallback(event.button.button);
				break;
		}
	}
}

EventHandler*& EventHandler::Get()
{
	return eventHandler;
}
