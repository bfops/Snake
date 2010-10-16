#pragma once

class Counter
{
private:
	unsigned int elapsedCount;

public:
	Counter();
	void Reset();
	void Update(unsigned int elapsedCount);
	/// if at least _count_ has elapsed, reset the counter
	/// (accounting for overflow if more than _count_ has elapsed).
	/// Returns true if the counter was reset, false otherwise.
	bool ResetIfHasElapsed(unsigned int count);
};
