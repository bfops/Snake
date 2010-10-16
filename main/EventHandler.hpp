#pragma once

#include "Common.hpp"

#include <SDL_events.h>

class GameState;
class ZippedUniqueObjectList;

class EventHandler
{
public:
	typedef void (QuitCallbackType)();
	typedef void (PauseCallbackType)();
	typedef void (KeyCallbackType)(SDLKey);
	typedef void (MouseCallbackType)(Uint8 mouseButton);

private:
#define DECLARE_CALLBACK_FUNCTOR(type, ltype) \
	type##CallbackType* const ltype##Callback;

	DECLARE_CALLBACK_FUNCTOR(Quit, quit)
	DECLARE_CALLBACK_FUNCTOR(Pause, pause)
	DECLARE_CALLBACK_FUNCTOR(Key, key)
	DECLARE_CALLBACK_FUNCTOR(Mouse, mouse)

#undef DECLARE_CALLBACK_FUNCTOR

public:
	EventHandler(QuitCallbackType, PauseCallbackType, KeyCallbackType, MouseCallbackType);

	void HandleEventQueue(GameState& gameState, ZippedUniqueObjectList& gameObjects) const;
};
