#pragma once

#define countof(array) (sizeof(array) / sizeof(array[0]))

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
#define DEBUGLOGIF(func, logger, string) ;
#endif

// random game-changing things //

// whether or not extra walls are added for epicness
#define COOLERWALLS
// whether or not music is on
#define MUSIC
