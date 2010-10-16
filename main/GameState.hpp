#pragma once

#include <SDL_events.h>

class GameWorld;
class ZippedUniqueObjectList;

class GameState
{
private:
	GameWorld& gameWorld;

	bool quit, paused;

public:
	GameState(GameWorld& gameWorld);

	void QuitHandler();
	void PauseHandler();

	bool IsPaused() const;
	bool QuitCalled() const;
};
