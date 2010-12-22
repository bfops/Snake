#include "Mutex.hpp"

Mutex::Mutex() :
	mutex(new boost::mutex())
{
}

Mutex::Mutex(const Mutex&) :
	mutex(new boost::mutex())
{
}

Mutex& Mutex::operator=(const Mutex&)
{
	return *this;
}

void Mutex::Lock() const
{
	mutex->lock();
}

void Mutex::Unlock() const
{
	mutex->unlock();
}

RecursiveMutex::RecursiveMutex() :
	mutex(new boost::recursive_mutex())
{
}

RecursiveMutex::RecursiveMutex(const RecursiveMutex&) :
	mutex(new boost::recursive_mutex())
{
}

RecursiveMutex& RecursiveMutex::operator=(const RecursiveMutex&)
{
	return *this;
}

void RecursiveMutex::Lock() const
{
	mutex->lock();
}

void RecursiveMutex::Unlock() const
{
	mutex->unlock();
}
