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

static inline void write(const char* type, const char* message)
{
	printf("[%s] %s\n", type, message);
}

namespace Logger
{
#ifndef NDEBUG
	void Debug(const char* message)
	{
		write("Debug", message);
	}
#endif

	void Fatal(const char* message)
	{
		write("Fatal", message);
		throw std::runtime_error(message);
	}
}
