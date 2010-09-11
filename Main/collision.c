#include "collision.h"

static inline bool within_bounds(int obj1Location, int obj2Location, unsigned int dimension)
{
	return((obj1Location >= obj2Location) && (obj1Location < (int)(obj2Location + dimension)));
}

static inline bool left_within_bounds(const CollidableObject* restrict obj1, const CollidableObject* restrict obj2)
{
	if(obj1->x < obj2->x)
		return within_bounds(obj2->x, obj1->x, obj1->width);

	return within_bounds(obj1->x, obj2->x, obj2->width);
}

static inline bool top_within_bounds(const CollidableObject * restrict obj1, const CollidableObject* restrict obj2)
{
	if(obj1->y < obj2->y)
		return within_bounds(obj2->y, obj1->y, obj1->height);

	return within_bounds(obj1->y, obj2->y, obj2->height);
}

bool does_collide(const CollidableObject* restrict o1, const CollidableObject* restrict o2)
{
	bool t1 = top_within_bounds(o1, o2);
	//bool t2 = top_within_bounds(o2, o1);
	bool t3 = left_within_bounds(o1, o2);
	//bool t4 = left_within_bounds(o2, o1);

	return (t1) && (t3);
}
