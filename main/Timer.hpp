#pragma once

class Timer
{
private:
	unsigned int lastTime;
	unsigned int elapsedMilliseconds;

public:
	Timer();

	void Reset();
	void Update();
	/// pretend to update, without updating elapsedMilliseconds count
	void SilentUpdate();
	/// return elapsed time since last call (in ms)
	unsigned int GetElapsedTime();
};
