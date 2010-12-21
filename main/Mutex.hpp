#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>

#ifdef MSVC
#pragma warning(pop)
#endif

// a mutex class which does NOT preserve mutex state when copying
class Mutex
{
private:
	boost::mutex mutex;

public:
	Mutex();
	Mutex(const Mutex& other);

	Mutex& operator=(const Mutex& other);

	void Lock();
	void Unlock();
};

// a recursive mutex class which does NOT preserve mutex state when copying
class RecursiveMutex
{
private:
	boost::recursive_mutex mutex;

public:
	RecursiveMutex();
	RecursiveMutex(const RecursiveMutex& other);

	RecursiveMutex& operator=(const RecursiveMutex& other);

	void Lock();
	void Unlock();
};
