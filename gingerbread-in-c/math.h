#pragma once
#include <math.h>

typedef struct
{
	float x;
	float y;
	float z;
} Point;

inline Point make_point(float x, float y, float z)
{
	Point ret = { x, y, z };
	return ret;
}

typedef struct
{
	float x;
	float y;
	float z;
} Vector;

inline Vector make_vector(float x, float y, float z)
{
	Vector ret = { x, y, z };
	return ret;
}

typedef union
{
	struct _nums
	{
	float _11, _12, _13, _14,
	      _21, _22, _23, _24,
	      _31, _32, _33, _34,
	      _41, _42, _43, _44;
	} nums;

	float data[4][4];
} Matrix;

float fast_sin(float x);
float fast_cos(float x);
float fast_tan(float x);
float fast_asin(float x);
float fast_acos(float x);
float fast_atan(float x);
