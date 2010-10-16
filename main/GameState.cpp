#include "GameState.hpp"

#include "GameWorld.hpp"

#include <SDL_timer.h>

GameState::GameState(GameWorld& world) :
	gameWorld(world)
{
	quit = false;
	paused = false;
}

void GameState::QuitHandler()
{
	quit = true;
}

void GameState::PauseHandler()
{
	paused = !paused;
}

bool GameState::IsPaused() const
{
	return paused;
}

bool GameState::QuitCalled() const
{
	return quit;
}
