#include "Timer.hpp"

#include <SDL_timer.h>

// may switch (back) to boost later, so this
// allows for easy switching of timing methods
static inline unsigned int get_current_time()
{
	return SDL_GetTicks();
}

Timer::Timer()
{
	Reset();
}

void Timer::Reset()
{
	elapsedMilliseconds = 0;
	lastTime = get_current_time();
}

void Timer::Update()
{
	const unsigned int newTime = get_current_time();
	elapsedMilliseconds += newTime - lastTime;
	lastTime = newTime;
}

void Timer::SilentUpdate()
{
	lastTime = get_current_time();
}

unsigned int Timer::GetElapsedTime()
{
	const unsigned int retval = elapsedMilliseconds;
	elapsedMilliseconds = 0;
	return retval;
}
