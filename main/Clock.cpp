#include "Clock.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <SDL_timer.h>

#ifdef MSVC
#pragma warning(pop)
#endif

static Clock clock;

static inline unsigned long get_raw_time()
{
	return SDL_GetTicks();
}

void Clock::UpdateTime()
{
	const unsigned long rawTime = get_raw_time();
	time += rawTime - lastTime;
	lastTime = rawTime;
}

Clock& Clock::Get()
{
	return clock;
}

Clock::Clock() :
	paused(false), time(0), lastTime(get_raw_time())
{
}

unsigned long Clock::GetTime()
{
	UpdateTime();
	return time;
}

void Clock::Pause()
{
	UpdateTime();
	paused = true;
}

void Clock::Unpause()
{
	lastTime = get_raw_time();
	paused = false;
}
