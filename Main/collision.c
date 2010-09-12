#include "collision.h"

bool does_collide(const CollidableObject* restrict obj1, const CollidableObject* restrict obj2)
{
	#define TESTBOUNDS(m) ((obj1->Min.m <= obj2->Min.m) ? (obj2->Min.m < obj1->Max.m) : (obj1->Min.m < obj2->Max.m))
	return (TESTBOUNDS(x) && TESTBOUNDS(y));
	#undef TESTBOUNDS
}
