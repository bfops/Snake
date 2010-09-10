#include <cassert>
#include <cstdarg>
#include <cstdio>

#include "Common.hpp"
#include "DebugLogger.hpp"

using namespace std;

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
		for(unsigned int i = 0; i < tabNumber; ++i)
			printf("\t");

		va_list argList;
		va_start(argList, fmt);
		vprintf(fmt, argList);
		va_end(argList);
		#endif
	}
}
