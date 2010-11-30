#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/date_time.hpp>

#ifdef MSVC
#pragma warning(pop)
#endif

class Clock
{
private:
	bool paused;
	unsigned long time;
	boost::posix_time::ptime lastTime;

	void UpdateTime();

public:
	static Clock& Get();

	Clock();

	unsigned long GetTime();
	void Pause();
	void Unpause();
};
