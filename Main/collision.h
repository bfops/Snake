#pragma once

#ifdef __cplusplus
	#define restrict __restrict__
	extern "C" {
#else
	#include <stdbool.h>
#endif

typedef struct
{
	int x;
	int y;
	unsigned int height;
	unsigned int width;
} CollidableObject;

bool does_collide(const CollidableObject* restrict o1, const CollidableObject* restrict o2);

#ifdef __cplusplus
	}
#endif
