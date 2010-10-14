#include "GameState.hpp"

#include "GameWorld.hpp"

#include <SDL_timer.h>

GameState::GameState(GameWorld& world) :
	gameWorld(world)
{
	quit = false;
	paused = false;
	start = SDL_GetTicks();
}

void GameState::QuitHandler()
{
	quit = true;
}

void GameState::PauseHandler()
{
	paused = !paused;
}

void GameState::KeyHandler(SDLKey key, ZippedUniqueObjectList& gameObjects)
{
	gameWorld.KeyNotify(key, gameObjects);
}

void GameState::MouseHandler(Uint8 mouseButton, ZippedUniqueObjectList& gameObjects)
{
	gameWorld.MouseNotify(mouseButton, gameObjects);
}

bool GameState::IsPaused() const
{
	return paused;
}

bool GameState::QuitCalled() const
{
	return quit;
}

void GameState::Update()
{
	if(paused)
	{
		start = SDL_GetTicks() - elapsedMilliseconds;
	}
	else
	{
		elapsedMilliseconds = SDL_GetTicks() - start;
		start = SDL_GetTicks();
	}
}

unsigned int GameState::GetElapsedTime() const
{
	return elapsedMilliseconds;
}
