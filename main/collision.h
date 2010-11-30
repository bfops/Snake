#pragma once

#ifdef __cplusplus
	#ifdef __GNUC__
	#define __restrict __restrict__
	#endif
	extern "C" {
#endif

typedef struct
{
	int x, y;
} point;

typedef struct
{
	point min;
	point max;
} CollidableObject;

int does_collide(const CollidableObject* const __restrict o1, const CollidableObject* const __restrict o2);

#ifdef __cplusplus
	}
#endif
