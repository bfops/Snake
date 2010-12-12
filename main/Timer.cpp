#include "Timer.hpp"

#include "Clock.hpp"

Timer::Timer()
{
	Reset();
}

void Timer::Reset()
{
	lastTime = Clock::Get().GetTime();
	elapsed = 0;
}

bool Timer::ResetIfHasElapsed(unsigned int c)
{
	const unsigned long currentTime = Clock::Get().GetTime();
	const unsigned long deltaT = currentTime - lastTime;
	elapsed += deltaT;
	lastTime = currentTime;

	if(elapsed >= c)
	{
		elapsed -= c;
		return true;
	}

	return false;
}
