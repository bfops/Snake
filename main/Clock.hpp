#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <ctime>

#ifdef MSVC
#pragma warning(pop)
#endif

// keeps track of how much game time has passed
class Clock
{
private:
	bool paused;
	unsigned long long time;
	// the time state, at the last time the time was requested
	clock_t lastTime;
	
	static Clock gameClock;
	
	Clock();
	void UpdateTime();

public:
	// get the (only) clock 
	static Clock& Get();

	unsigned long long GetTime();

	void Pause();
	void Unpause();
};
