#pragma once

#include <SDL_events.h>

class GameWorld;
class ZippedUniqueObjectList;

class GameState
{
private:
	GameWorld& gameWorld;

	bool quit;

public:
	GameState(GameWorld& gameWorld);

	void QuitHandler();
	bool QuitCalled() const;
};
