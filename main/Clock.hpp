#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/date_time.hpp>

#ifdef MSVC
#pragma warning(pop)
#endif

// keeps track of how much game time has passed
class Clock
{
private:
	bool paused;
	unsigned long time;
	// the time state, at the last time the time was requested
	boost::posix_time::ptime lastTime;
	
	static Clock gameClock;
	
	Clock();
	void UpdateTime();

public:
	// get the (only) clock 
	static Clock& Get();

	unsigned long GetTime();

	void Pause();
	void Unpause();
};
