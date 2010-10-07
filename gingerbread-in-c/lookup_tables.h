#pragma once
#include <stddef.h>

typedef struct
{
	const float* restrict table;
	size_t len;
} LookupTable;

// For a lookup table for cosx, use a transformation of sinx!

// x E [0, pi/2], 4 significant decimal places
extern const LookupTable* restrict sinLookupTable;

// x E [0, 1], 4 significant decimal places
extern const LookupTable* restrict asinLookupTable;

// arctan doesn't get a lookup table, since it has a valid domain of [-inf, inf]
// and is not periodic. Just call math.h's atan.
