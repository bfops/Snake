#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/thread/recursive_mutex.hpp>
#include <SDL_events.h>

#ifdef MSVC
#pragma warning(pop)
#endif

struct ZippedUniqueObjectList;

// when specific events happen, it redirects calls to their
// respective callback functions.
// Depends on SDL for event data
struct EventHandler
{
	typedef void (QuitCallbackType)();
	typedef void (LossCallbackType)();
	typedef void (PauseCallbackType)();
	typedef void (KeyCallbackType)(SDLKey keyPressed);
	typedef void (MouseCallbackType)(Uint8 mouseButton);

	boost::recursive_mutex mutex;

#define DECLARE_CALLBACK_FUNCTOR(type) \
	type##CallbackType* ##type##Callback;

	DECLARE_CALLBACK_FUNCTOR(Quit)
	DECLARE_CALLBACK_FUNCTOR(Loss)
	DECLARE_CALLBACK_FUNCTOR(Pause)
	DECLARE_CALLBACK_FUNCTOR(Key)
	DECLARE_CALLBACK_FUNCTOR(Mouse)

#undef DECLARE_CALLBACK_FUNCTOR

	EventHandler(QuitCallbackType, LossCallbackType, PauseCallbackType, KeyCallbackType, MouseCallbackType);

	// get and handle the queue of events from SDL
	void HandleEventQueue() const;

	static EventHandler*& Get();
};
