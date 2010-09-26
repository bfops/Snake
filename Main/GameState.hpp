#pragma once

#include <SDL/SDL_events.h>

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
	void KeyHandler(SDLKey key, ZippedUniqueObjectList& gameObjects);
	void MouseHandler(Uint8 mouseButton, ZippedUniqueObjectList& gameObjects);

	bool IsPaused() const;
	bool QuitCalled() const;
};
