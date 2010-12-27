#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <ctime>

#ifdef MSVC
#pragma warning(pop)
#endif

// keeps track of game time (starting at 0)
class Clock
{
public:
	typedef unsigned long long TimeType;

private:
	bool paused;
	TimeType time;
	// the time state, at the last time the time was requested
	clock_t lastTime;
	
	static Clock gameClock;
	
	Clock();
	void UpdateTime();

public:
	// get the (only) clock 
	static Clock& Get();

	TimeType GetTime();

	void Pause();
	void Unpause();
};
