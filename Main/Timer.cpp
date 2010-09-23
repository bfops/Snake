#include "Timer.hpp"

#include "Common.hpp"

using namespace boost::posix_time;

Timer::Timer()
{
	Reset();
}

void Timer::Reset()
{
	begin = microsec_clock::local_time();
}

bool Timer::ResetIfHasElapsed(unsigned int ms)
{
	const ptime now = microsec_clock::local_time();
	ptime destTime = begin + milliseconds(ms);
	time_duration diff = now - destTime;

	if(!diff.is_negative())
	{
		begin = now - diff;
		return true;
	}

	return false;
}
