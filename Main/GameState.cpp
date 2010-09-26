#include "GameState.hpp"

#include "Common.hpp"
#include "GameWorld.hpp"

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
