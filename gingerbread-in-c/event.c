#include "event.h"

#include <SDL/SDL.h>

#include <assert.h>
#include <stddef.h>

#define MAX_EVENTS 512
static EventHandler handlers[MAX_EVENTS];
static EventHandler* end = handlers;

bool event_init()
{
	memset(handlers, sizeof(handlers), 0);
	return true;
}

void event_add_handler(EventHandler h)
{
	assert(
		"Too many event handlers! Try increasing maxEvents."
		&& end != handlers + MAX_EVENTS
	);
	*end++ = h;
}

static EventHandler* find_handler(EventHandler* haystack, EventHandler* end, void (*needle)(void*))
{
	assert("What the shit? Invalid range, nigga." && haystack <= end);

	for(EventHandler* h = haystack; h < end; ++h)
		if(h->handler == needle)
			return h;

	return NULL;
}

static void remove_handler_impl(EventHandler* begin, EventHandler* end, void (*h)(void*))
{
	EventHandler* loc = find_handler(begin, end, h);

	if(loc)
	{
		*loc = *--end;
		remove_handler_impl(loc, end, h);
	}
}

void event_remove_handler_by_functor(void (*h)(void*))
{
	remove_handler_impl(handlers, end, h);
}

static EventHandler* find_handler_by_predicate(EventHandler* begin, EventHandler* end, bool (*should_remove)(const EventHandler*, void*), void* param)
{
	assert("What the shit? Invalid range, nigga." && begin <= end);

	for(EventHandler* h = begin; h != end; ++h)
		if(should_remove(h, param))
			return h;

	return NULL;
}

static void remove_handler_by_predicate_impl(EventHandler* begin, EventHandler* end, bool (*should_remove)(const EventHandler*, void*), void* param)
{
	EventHandler* loc = find_handler_by_predicate(begin, end, should_remove, param);

	if(loc)
	{
		*loc = *--end;
		remove_handler_by_predicate_impl(loc, end, should_remove, param);
	}
}

void event_remove_handler_by_predicate(bool (*should_remove)(const EventHandler* e, void* param), void* param)
{
	remove_handler_by_predicate_impl(handlers, end, should_remove, param);
}

static EventType detect_event_type(const SDL_Event* e)
{
	switch(e->type)
	{
		case SDL_QUIT: return EVENT_QUIT;
		case SDL_KEYDOWN: return e->key.keysym.sym;
	}

	return EVENT_UNKNOWN;
}

void event_tick()
{
	SDL_Event e;

	if(!SDL_PollEvent(&e))
		return;

	EventType t = detect_event_type(&e);

	for(EventHandler* h = handlers; h != end; ++h)
		if(h->type == t)
			h->handler(h->param);
}

void event_shutdown()
{
}
