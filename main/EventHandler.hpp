#pragma once

#include "Mutex.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <SDL_events.h>

#ifdef MSVC
#pragma warning(pop)
#endif

struct ZippedUniqueObjectCollection;

// when specific events happen, it redirects calls to their
// respective callback functions.
// Depends on SDL for event data
struct EventHandler
{
	typedef void (QuitCallbackType)();
	typedef void (LossCallbackType)();
	typedef void (PauseCallbackType)();
	typedef void (SoundCallbackType)(const std::string& filename);
	typedef void (KeyCallbackType)(SDLKey keyPressed);
	typedef void (MouseCallbackType)(Uint8 mouseButton);

	static RecursiveMutex mutex;

#define DECLARE_CALLBACK_FUNCTOR(type) \
	type##CallbackType* type##Callback;

	DECLARE_CALLBACK_FUNCTOR(Quit)
	DECLARE_CALLBACK_FUNCTOR(Loss)
	DECLARE_CALLBACK_FUNCTOR(Pause)
	DECLARE_CALLBACK_FUNCTOR(Sound)
	DECLARE_CALLBACK_FUNCTOR(Key)
	DECLARE_CALLBACK_FUNCTOR(Mouse)

#undef DECLARE_CALLBACK_FUNCTOR

	EventHandler(QuitCallbackType, LossCallbackType, PauseCallbackType, SoundCallbackType, KeyCallbackType,
		MouseCallbackType);

	// get and handle the queue of events from SDL
	void HandleEventQueue() const;

	static const EventHandler*& Get();
};
