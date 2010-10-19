#pragma once

#include "Common.hpp"

#include <SDL_events.h>

class ZippedUniqueObjectList;

class EventHandler
{
public:
	typedef void (QuitCallbackType)();
	typedef void (LossCallbackType)();
	typedef void (PauseCallbackType)();
	typedef void (KeyCallbackType)(SDLKey keyPressed);
	typedef void (MouseCallbackType)(Uint8 mouseButton);

private:
#define DECLARE_CALLBACK_FUNCTOR(type, ltype) \
	type##CallbackType* const ltype##Callback;

	DECLARE_CALLBACK_FUNCTOR(Quit, quit)
	DECLARE_CALLBACK_FUNCTOR(Loss, loss)
	DECLARE_CALLBACK_FUNCTOR(Pause, pause)
	DECLARE_CALLBACK_FUNCTOR(Key, key)
	DECLARE_CALLBACK_FUNCTOR(Mouse, mouse)

#undef DECLARE_CALLBACK_FUNCTOR

public:
	EventHandler(QuitCallbackType, LossCallbackType, PauseCallbackType, KeyCallbackType, MouseCallbackType);

	void HandleEventQueue(ZippedUniqueObjectList& gameObjects) const;
	void LossNotify() const;

	static const EventHandler*& GetCurrentEventHandler();
};
