#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/scoped_ptr.hpp>

#ifdef MSVC
#pragma warning(pop)
#endif

// a mutex class which does NOT preserve mutex state when copying
class Mutex
{
private:
	boost::scoped_ptr<boost::mutex> mutex;

public:
	Mutex();
	Mutex(const Mutex& other);

	Mutex& operator=(const Mutex& other);

	void Lock() const;
	void Unlock() const;
};

// a recursive mutex class which does NOT preserve mutex state when copying
class RecursiveMutex
{
private:
	boost::scoped_ptr<boost::recursive_mutex> mutex;

public:
	RecursiveMutex();
	RecursiveMutex(const RecursiveMutex& other);

	RecursiveMutex& operator=(const RecursiveMutex& other);

	void Lock() const;
	void Unlock() const;
};
