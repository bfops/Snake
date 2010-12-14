#include "Logger.hpp"

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <cstdio>
#include <stdexcept>

#ifdef MSVC
#pragma warning(pop)
#endif

static inline void write(const char* const type, const char* const message)
{
	printf("[%s] %s\n", type, message);
}

namespace Logger
{
#ifdef NDEBUG
	void Debug(const char* const)
	{
	}

	void Debug(const boost::format&)
	{
	}
#else
	void Debug(const char* const message)
	{
		write("Debug", message);
	}

	void Debug(const boost::format& message)
	{
		return Debug(message.str().c_str());
	}
#endif

	void Fatal(const char* const message)
	{
		write("Fatal", message);
		throw std::runtime_error(message);
	}

	void Fatal(const boost::format& message)
	{
		return Fatal(message.str().c_str());
	}
}
