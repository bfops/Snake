#pragma once

#define countof(array) (sizeof(array) / sizeof(array[0]))

// round to nearest integer
static inline long intRound(const double d)
{
	return ((d < 0) ? (d - 0.5) : (d + 0.5));
}

// execute _thingsToDo_ while _mutex_ is locked
#define DOLOCKED(mutex, thingsToDo) \
	mutex.Lock(); \
	thingsToDo \
	mutex.Unlock();
