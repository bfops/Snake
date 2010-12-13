#pragma once

// basic functionality for checking if intervals have elapsed
class Timer
{
private:
	// the time, last time the time was checked
	unsigned long lastTime;
	// the amount of time elapsed
	unsigned long elapsed;

public:
	Timer();
	void Reset();
	// if at least _count_ has elapsed, reset the counter
	// (accounting for overflow if more than _count_ has elapsed).
	// Returns true if the counter was reset, false otherwise.
	bool ResetIfHasElapsed(unsigned int ms);
};
