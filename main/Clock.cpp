#include "Clock.hpp"

Clock Clock::gameClock;

static inline clock_t get_raw_time()
{
	return clock();
}

void Clock::UpdateTime()
{
	const clock_t rawTime = get_raw_time();
	const clock_t deltaT = (rawTime - lastTime) * 1000 / CLOCKS_PER_SEC;

	time += deltaT;
	lastTime = rawTime;
}

Clock& Clock::Get()
{
	return gameClock;
}

Clock::Clock()
{
	paused = false;
	time = 0;
	lastTime = get_raw_time();
}

Clock::TimeType Clock::GetTime()
{
	if(!paused)
		UpdateTime();

	return time;
}

void Clock::Pause()
{
	paused = true;
	UpdateTime();
}

void Clock::Unpause()
{
	paused = false;
	lastTime = get_raw_time();
}
