#include "GameState.hpp"

#include "GameWorld.hpp"

#include <SDL_timer.h>

GameState::GameState(GameWorld& world) :
	gameWorld(world)
{
	quit = false;
}

void GameState::QuitHandler()
{
	quit = true;
}

bool GameState::QuitCalled() const
{
	return quit;
}
