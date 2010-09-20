#pragma once

#define countof(array) (sizeof(array) / sizeof(array[0]))

#define DEF_CONSTANT(type, name, value) \
	inline type name() { return value; }

#ifndef NDEBUG
#define DEBUG
#endif

#ifdef DEBUG
#define DEBUGLOG(logger, string) \
	logger.Debug(string);
#else
#define DEBUGLOG(logger, string) ;
#endif

#ifdef DEBUG
#define DEBUGLOGIF(func, logger, string) \
	if(func) \
		DEBUGLOG(logger, string)
#else
#define DEBUGLOG(func, logger, __FILE__ ## " #" # __LINE__ ## ": " ## string) ;
#endif
