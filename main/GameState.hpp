#pragma once

#include <boost/date_time.hpp>
#include <SDL/SDL_events.h>

class GameWorld;
class ZippedUniqueObjectList;

class GameState
{
private:
	GameWorld& gameWorld;

	bool quit, paused;

	boost::posix_time::ptime start;
	unsigned int elapsedMilliseconds;

public:
	GameState(GameWorld& gameWorld);

	void QuitHandler();
	void PauseHandler();
	void KeyHandler(SDLKey key, ZippedUniqueObjectList& gameObjects);
	void MouseHandler(Uint8 mouseButton, ZippedUniqueObjectList& gameObjects);

	bool IsPaused() const;
	bool QuitCalled() const;

	void Update();
	unsigned int GetElapsedTime() const;
};
