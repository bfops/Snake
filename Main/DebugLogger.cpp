#include <cassert>
#include <cstdarg>

#include "DebugLogger.hpp"

namespace DebugLogger {

	#ifndef NDEBUG
	namespace {
	unsigned int tabNumber;

	struct Initializer
	{
		Initializer()
		{
			tabNumber = 0;
		}
	};
	}
	#endif

	Indent::Indent()
	{
		#ifndef NDEBUG
		++tabNumber;
		#endif
	}
	Indent::~Indent()
	{
		#ifndef NDEBUG
		--tabNumber;
		#endif
	}

	void Log(const char* fmt, ...)
	{
		#ifndef NDEBUG
		assert(!"DebugLogger::Log");
		#endif
	}
}
