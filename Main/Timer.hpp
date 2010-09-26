#pragma once

class Timer
{
private:
	unsigned int elapsedMilliseconds;

public:
	Timer();
	void Reset();
	void Update(unsigned int elapsedMilliseconds);
	/// if at least _ms_ milliseconds have elapsed,
	/// reset the timer (accounting for overflow if
	/// more than _ms_ milliseconds have elapsed).
	/// Returns whether or not the timer was reset.
	bool ResetIfHasElapsed(unsigned int ms);
};
