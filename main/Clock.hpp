#pragma once

class Clock
{
private:
	bool paused;
	unsigned long time;
	unsigned long lastTime;

	void UpdateTime();

public:
	static Clock& Get();

	Clock();

	unsigned long GetTime();
	void Pause();
	void Unpause();
};
