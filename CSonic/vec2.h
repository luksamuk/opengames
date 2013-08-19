#ifndef VEC2_H
#define VEC2_H

#include "types.h"

typedef struct VEC2_DW_DEF
{
	dword x, y;
} vec2;

typedef struct VEC2_B_DEF
{
	int x, y;
} vec2_b;

vec2 new_vec2(dword, dword);

#endif // VEC2_H