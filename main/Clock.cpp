#include "Clock.hpp"

static Clock gameClock;

static inline boost::posix_time::ptime get_raw_time()
{
	return boost::posix_time::microsec_clock::universal_time();
}

void Clock::UpdateTime()
{
	const boost::posix_time::ptime rawTime = get_raw_time();
	time += (rawTime - lastTime).total_milliseconds();
	lastTime = rawTime;
}

Clock& Clock::Get()
{
	return gameClock;
}

Clock::Clock() :
	paused(false), time(0), lastTime(get_raw_time())
{
}

unsigned long Clock::GetTime()
{
	if(!paused)
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
