#pragma once

#define countof(array) (sizeof(array) / sizeof(array[0]))

inline int round(const double d)
{
	return int(d + 0.5);
}

#ifndef NDEBUG
#define DEBUG
#endif

#define DOLOCKED(mutex, thingsToDo) mutex.lock(); \
	thingsToDo \
	mutex.unlock();
