#include "Mutex.hpp"

Mutex::Mutex()
{
}

Mutex::Mutex(const Mutex&)
{
}

Mutex& Mutex::operator=(const Mutex&)
{
	return *this;
}

void Mutex::Lock()
{
	mutex.lock();
}

void Mutex::Unlock()
{
	mutex.unlock();
}

RecursiveMutex::RecursiveMutex()
{
}

RecursiveMutex::RecursiveMutex(const RecursiveMutex&)
{
}

RecursiveMutex& RecursiveMutex::operator=(const RecursiveMutex&)
{
	return *this;
}

void RecursiveMutex::Lock()
{
	mutex.lock();
}

void RecursiveMutex::Unlock()
{
	mutex.unlock();
}
