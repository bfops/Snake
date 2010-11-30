#include "Logger.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <cstdio>
#include <stdexcept>
#include <boost/thread/mutex.hpp>

#ifdef MSVC
#pragma warning(pop)
#endif

typedef boost::mutex Mut;
typedef boost::unique_lock<Mut> Lock;

static Mut ioProtection;

static void write(const char* type, const char* tag, const char* message)
{
	Lock w(ioProtection);
	printf("%s -> [%s]: %s\n", type, tag, message);
}

namespace Logger
{
#ifndef NDEBUG
	void Handle::Debug(const char* message) const
	{
		write("D", tag, message);
	}
#endif

	void Handle::Fatal(const char* message) const
	{
		write("F", tag, message);
		throw std::runtime_error(message);
	}
}
