#include <cassert>

#include "Common.hpp"

#ifndef NDEBUG
// uncomment for debug output
#define DEBUGOUTPUT
#endif

void DebugOutput(const char* format, ...)
{
	#ifdef DEBUGOUTPUT
	assert(!"DebugOutput");
	#endif
}