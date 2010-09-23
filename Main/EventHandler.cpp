#include "EventHandler.hpp"

#include "Common.hpp"
#include "GameWorld.hpp"

void EventHandler::Update(GameWorld& world, UniqueObjectList& gameObjects)
{
	SDL_Event event;

	if(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				world.KeyNotify(event.key.keysym.sym, gameObjects);
				break;

			case SDL_QUIT:
				world.QuitNotify();
				break;
		}
	}
}
