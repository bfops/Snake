#pragma once

#define countof(array) (sizeof(array) / sizeof(array[0]))

#define DEF_CONSTANT(type, name, value) \
	static inline type name() { return value; }

#ifndef NDEBUG
#define DEBUG
#endif
