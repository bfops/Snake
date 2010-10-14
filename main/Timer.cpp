#include "Timer.hpp"

Timer::Timer()
{
	Reset();
}

void Timer::Reset()
{
	elapsedMilliseconds = 0;
}

void Timer::Update(unsigned int milliseconds)
{
	elapsedMilliseconds += milliseconds;
}

bool Timer::ResetIfHasElapsed(unsigned int ms)
{
	if(elapsedMilliseconds >= ms)
	{
		elapsedMilliseconds -= ms;
		return true;
	}

	return false;
}
