#include "collision.h"

static inline bool within_bounds(int obj1Location, int obj2Location, unsigned int dimension)
{
	return((obj1Location >= obj2Location) && (obj1Location < (int)(obj2Location + dimension)));
}

static inline bool left_within_bounds(const CollidableObject* restrict obj1, const CollidableObject* restrict obj2)
{
	return within_bounds(obj1->x, obj2->x, obj2->width);
}

static inline bool top_within_bounds(const CollidableObject * restrict obj1, const CollidableObject* restrict obj2)
{
	return within_bounds(obj1->y, obj2->y, obj2->height);
}

bool does_collide(const CollidableObject* restrict o1, const CollidableObject* restrict o2)
{
	return
	(
		(top_within_bounds (o1, o2) || top_within_bounds (o2, o1))
	 && (left_within_bounds(o1, o2) || left_within_bounds(o2, o1))
	);
}
