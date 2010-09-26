#include "math.h"
#include "lookup_tables.h"

#include <assert.h>
#include <math.h>

#define PI 3.14159265359f

// Takes a value `x` in the floating domain [0, inMax] and projects it into the
// integral range [0, outMax].
static inline size_t interpolate(float x, float inMax, size_t outMax)
{
	assert(x >= 0 && x <= inMax);
	return (size_t)(x * outMax / inMax);
}

// TODO: Turn this into some good ole' fashioned SSE assembly.
float fast_sin(float x)
{
	// Since sin is periodic over 2pi, we just recurse when we get a number
	// out of our first period. Now we only have to worry about -2pi <= x <= 2pi
	if(x < -2*PI || x > 2*PI)
		return fast_sin(x / 2*PI);

	// Horray for odd functions. Now, we only have to worry about 0 <= x <= 2pi
	// Or, exactly 1 period :)
	if(x < 0)
		return -fast_sin(-x);

	// Now we only need to worry about 0 <= x <= pi
	if(x > PI)
		return -fast_sin(x - PI);

	// And with this last test, we have a lookup-able range of 0 <= x <= pi/2
	if(x > PI/2)
		return fast_sin(PI/2 - x);

	// Finally, we get to the lookup table!
	return sinLookupTable->table[interpolate(x, PI/2, sinLookupTable->len - 1)];
}

float fast_cos(float x)
{
	return fast_sin(x + PI / 2);
}

float fast_tan(float x)
{
	return fast_sin(x)/fast_cos(x);
}

float fast_asin(float x)
{
	assert(x >= -1.0f && x <= 1.0f);

	// asin(x) is an odd function. Deal with it.
	if(x < 0)
		return -fast_asin(-x);

	return asinLookupTable->table[interpolate(x, 1, asinLookupTable->len - 1)];
}

float fast_acos(float x)
{
	return fast_asin(-x) + PI/2;
}

float fast_atan(float x)
{
	return atan(x);
}

