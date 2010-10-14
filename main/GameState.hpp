#pragma once

#include <SDL_events.h>

class GameWorld;
class ZippedUniqueObjectList;

class GameState
{
private:
	GameWorld& gameWorld;

	bool quit, paused;

	Uint32 start;
	Uint32 elapsedMilliseconds;

public:
	GameState(GameWorld& gameWorld);

	void QuitHandler();
	void PauseHandler();
	void KeyHandler(SDLKey key, ZippedUniqueObjectList& gameObjects);
	void MouseHandler(Uint8 mouseButton, ZippedUniqueObjectList& gameObjects);

	bool IsPaused() const;
	bool QuitCalled() const;

	void Update();
	/// returns elapsed time since last call (in ms)
	unsigned int GetElapsedTime() const;
};
