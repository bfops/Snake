#pragma once

#define countof(array) (sizeof(array) / sizeof(array[0]))

inline int round(const double d)
{
	return int(d + 0.5);
}

// execute _thingsToDo_ while _mutex_ is locked
#define DOLOCKED(mutex, thingsToDo) \
	mutex.lock(); \
	thingsToDo \
	mutex.unlock();
