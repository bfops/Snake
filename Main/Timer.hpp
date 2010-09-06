#pragma once

#include "boost/date_time.hpp"

class Timer
{
private:
	boost::posix_time::ptime begin;

public:
	Timer();
	void Reset();
	// if at least [ms] milliseconds have elapsed,
	// reset the timer (accounting for overflow if
	// more than [ms] milliseconds have elapsed).
	// Returns whether or not the timer was reset.
	bool ResetIfHasElapsed(unsigned int ms);
};
