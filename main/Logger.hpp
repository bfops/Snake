#pragma once

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <boost/format.hpp>

#ifdef MSVC
#pragma warning(pop)
#endif

namespace Logger
{
	// log debug output
	void Debug(const char* const message);
	void Debug(const boost::format& message);

	// log fatal errors
	void Fatal(const char* const message);
	void Fatal(const boost::format& message);
};
