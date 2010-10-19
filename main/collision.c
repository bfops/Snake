#include "collision.h"

bool does_collide(const CollidableObject* const restrict obj1, const CollidableObject* const restrict obj2)
{
	#define TESTBOUNDS(m) (obj1->min.m < obj2->max.m && obj2->min.m < obj1->max.m)
	return (TESTBOUNDS(x) && TESTBOUNDS(y));
	#undef TESTBOUNDS
}
