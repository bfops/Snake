#include "Timer.hpp"

Timer::Timer()
{
	Reset();
}

void Timer::Reset()
{
	lastTime = Clock::Get().GetTime();
	elapsed = 0;
}

bool Timer::ResetIfHasElapsed(const Clock::TimeType c)
{
	const Clock::TimeType currentTime = Clock::Get().GetTime();
	const Clock::TimeType deltaT = currentTime - lastTime;
	elapsed += deltaT;
	lastTime = currentTime;

	if(elapsed >= c)
	{
		elapsed -= c;
		return true;
	}

	return false;
}
