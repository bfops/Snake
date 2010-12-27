#pragma once

#define countof(array) (sizeof(array) / sizeof(array[0]))

// round to nearest integer
static inline long intRound(const double d)
{
	return ((d < 0) ? (d - 0.5) : (d + 0.5));
}

// lock _mutex_, execute _thingsToDo_, and unlock _mutex_
#define DOLOCKED(mutex, thingsToDo) \
	mutex.Lock(); \
	thingsToDo \
	mutex.Unlock();
