#pragma once

#include "Common.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/thread/mutex.hpp>
#include <SDL_events.h>

#ifdef MSVC
#pragma warning(pop)
#endif

struct ZippedUniqueObjectList;

class EventHandler
{
public:
	typedef void (QuitCallbackType)();
	typedef void (LossCallbackType)();
	typedef void (PauseCallbackType)();
	typedef void (KeyCallbackType)(SDLKey keyPressed);
	typedef void (MouseCallbackType)(Uint8 mouseButton);

private:
	EventHandler& operator=(const EventHandler&);

#define DECLARE_CALLBACK_FUNCTOR(type, ltype) \
	type##CallbackType* const ltype##Callback;

	DECLARE_CALLBACK_FUNCTOR(Quit, quit)
	DECLARE_CALLBACK_FUNCTOR(Loss, loss)
	DECLARE_CALLBACK_FUNCTOR(Pause, pause)
	DECLARE_CALLBACK_FUNCTOR(Key, key)
	DECLARE_CALLBACK_FUNCTOR(Mouse, mouse)

#undef DECLARE_CALLBACK_FUNCTOR

public:
	static boost::mutex mutex;

	EventHandler(QuitCallbackType, LossCallbackType, PauseCallbackType, KeyCallbackType, MouseCallbackType);

	void HandleEventQueue() const;
	void LossNotify() const;

	static const EventHandler*& Get();
};
