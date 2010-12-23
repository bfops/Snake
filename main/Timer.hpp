#pragma once

#include "Clock.hpp"

// basic functionality for checking if intervals have elapsed
class Timer
{
private:
	// the time, last time the time was checked
	Clock::TimeType lastTime;
	// the amount of time elapsed
	Clock::TimeType elapsed;

public:
	Timer();
	void Reset();
	// if at least _count_ has elapsed, reset the counter
	// (accounting for overflow if more than _count_ has elapsed).
	// Returns true if the counter was reset, false otherwise.
	bool ResetIfHasElapsed(Clock::TimeType ms);
};
