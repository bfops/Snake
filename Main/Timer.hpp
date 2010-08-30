#ifndef __Timer__
#define __Timer__

#include "boost/date_time.hpp"

class Timer
{
private:
	boost::posix_time::ptime begin;

public:
	Timer();
	void Reset();
	bool ResetIfHasElapsed(unsigned int ms);
};

#endif