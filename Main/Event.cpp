#include "GameWorld.hpp"

#include "Common.hpp"
#include "Direction.hpp"

GameWorld::EventHandler::EventHandler(GameWorld& _world) :
	world(&_world)
{
}

void GameWorld::EventHandler::Update(UniqueObjectList& gameObjects)
{
	SDL_Event event;

	if(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				world->KeyNotify(event.key.keysym.sym, gameObjects);
				break;

			case SDL_QUIT:
				world->QuitNotify();
				break;
		}
	}
}
