#pragma once

#define countof(array) (sizeof(array) / sizeof(array[0]))

#define DEF_CONSTANT(type, name, value) \
	inline type name() { return value; }
