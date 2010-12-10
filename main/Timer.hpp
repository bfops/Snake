#pragma once

class Timer
{
private:
	unsigned long lastTime;
	unsigned long elapsed;

public:
	Timer();
	void Reset();
	// if at least _count_ has elapsed, reset the counter
	// (accounting for overflow if more than _count_ has elapsed).
	// Returns true if the counter was reset, false otherwise.
	bool ResetIfHasElapsed(unsigned int ms);
};
