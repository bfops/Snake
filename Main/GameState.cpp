#include "GameState.hpp"

#include "Common.hpp"
#include "GameWorld.hpp"

using namespace boost::posix_time;

static inline ptime get_current_time()
{
	return microsec_clock::local_time();
}

GameState::GameState(GameWorld& world) :
	gameWorld(world)
{
	quit = false;
	paused = false;
	start = get_current_time();
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
		start = get_current_time() - milliseconds(elapsedMilliseconds);
	}
	else
	{
		elapsedMilliseconds = (get_current_time() - start).total_milliseconds();
		start = get_current_time();
	}
}

unsigned int GameState::GetElapsedTime() const
{
	return elapsedMilliseconds;
}
