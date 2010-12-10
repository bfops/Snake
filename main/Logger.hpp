#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/format.hpp>
#include <cassert>
#include <cstdio>

#ifdef MSVC
#pragma warning(pop)
#endif

namespace Logger
{
#ifdef NDEBUG
	inline void Debug(const char*) {}
	inline void Debug(const boost::format&) {}
#else
	void Debug(const char* message);
	inline void Debug(const boost::format& message) { return Debug(message.str().c_str()); }
#endif

	void Fatal(const char* message);
	inline void Fatal(const boost::format& message) { return Fatal(message.str().c_str()); }
};
