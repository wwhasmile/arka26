#ifndef HAS_MATHS_H
#define HAS_MATHS_H

#include <defines.h>

#define MATH_PI (3.14159265358979323846)
#define MATH_TAU (6.28318530717958647692)
#define MATH_PI_HALF (1.57079632679489661923)
#define MATH_PI_QUARTER (0.785398163397448309616)
#define MATH_1_PI (0.318309886183790671538)
#define MATH_2_PI (0.636619772367581343076)
#define MATH_E (2.71828182845904523536)
#define MATH_LOG2E (1.44269504088896340736)
#define MATH_LOG10E (0.434294481903251827651)
#define MATH_LN2 (0.693147180559945309417)
#define MATH_LN10 (2.30258509299404568402)
#define MATH_SQRT2 (1.41421356237309504880))
#define MATH_INVSQRT2 (0.707106781186547524401)
#define MATH_EPSILON (1e-5)
#define MATH_DEG2RAD (MATH_PI / 180.0)
#define MATH_RAD2DEG (180.0 / MATH_PI)

typedef struct {
	f32 x;
	f32 y;
} float2;

typedef struct {
	f32 x;
	f32 y;
	f32 z;
} float3;

typedef struct {
	f32 x;
	f32 y;
	f32 z;
	f32 w;
} float4;

typedef union {
	f32 data[16];
	struct {
		float4 i;
		float4 j;
		float4 k;
		float4 l;
	} columns;
} float4x4;

#endif // HAS_MATHS_H
