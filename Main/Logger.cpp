#include "Logger.hpp"

#include <cstdio>

#include <boost/thread/mutex.hpp>

typedef boost::mutex Mut;
typedef boost::unique_lock<Mut> Lock;

static Mut ioProtection;

static void Write(const char* type, const char* tag, const char* message)
{
	Lock w(ioProtection);
	printf("%s -> [%s]: %s\n", type, tag, message);
}

namespace Logger
{
#ifndef NDEBUG
	void Handle::Debug(const char* message)
	{
		Write("D", tag, message);
	}
#endif

	void Handle::Fatal(const char* message)
	{
		Write("F", tag, message);
	}
}