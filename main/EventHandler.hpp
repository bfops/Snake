#pragma once

#include "Common.hpp"

#include <SDL_events.h>

class GameState;
class ZippedUniqueObjectList;

class EventHandler
{
public:
	typedef void (*QuitCallbackType)();
	typedef void (*PauseCallbackType)();
	typedef void (*KeyCallbackType)(SDLKey);
	typedef void (*MouseCallbackType)(Uint8 mouseButton);

	// Declares a Callback Variable to store a callback function,
	// And Defines a Registration Function to allow setting of the variable
#define DCVADRF(type, ltype) \
private: \
	type##CallbackType ltype##Callback; \
public: \
	inline void Register##type##Callback(type##CallbackType _##ltype##Callback) \
	{ \
		ltype##Callback = _##ltype##Callback; \
	}

	DCVADRF(Quit, quit)
	DCVADRF(Pause, pause)
	DCVADRF(Key, key)
	DCVADRF(Mouse, mouse)

#undef DCVADRF

public:
	void HandleEventQueue(GameState& gameState, ZippedUniqueObjectList& gameObjects);
};
