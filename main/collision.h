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
} ObjectBounds;

// return true iff o1 and o2 overlap
int does_collide(const ObjectBounds* const __restrict o1, const ObjectBounds* const __restrict o2);

#ifdef __cplusplus
}
#endif
