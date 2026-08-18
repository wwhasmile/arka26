#ifndef HAS_MATH_H
#define HAS_MATH_H

#include <defines.h>

#include <math.h>

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

inline f32 Math_Fmod(f32 a, f32 b);

inline f32 Math_Sqrt(f32 x);
inline f32 Math_Pow(f32 x, f32 exp);
inline f32 Math_Exp(f32 x);
inline f32 Math_Log(f32 x);
inline f32 Math_Log2(f32 x);
inline f32 Math_Log10(f32 x);

inline i32 Math_Floor(f32 x);
inline f32 Math_Ffloor(f32 x);
inline i32 Math_Round(f32 x);
inline f32 Math_Fround(f32 x);
inline i32 Math_Ceil(f32 x);
inline f32 Math_Fceil(f32 x);

inline i32 Math_Abs(i32 x);
inline f32 Math_Fabs(f32 x);
inline i32 Math_Sign(i32 x);
inline f32 Math_Fsign(f32 x);
inline i32 Math_Min(i32 x, i32 y);
inline f32 Math_Fmin(f32 x, f32 y);
inline i32 Math_Max(i32 x, i32 y);
inline f32 Math_Fmax(f32 x, f32 y);
inline i32 Math_Clamp(i32 x, i32 min, i32 max);
inline f32 Math_Fclamp(f32 x, f32 min, f32 max);
inline f32 Math_Remap(f32 x, f32 minA, f32 maxA, f32 minB, f32 maxB);
inline f32 Math_RemapClamped(f32 x, f32 minA, f32 maxA, f32 minB, f32 maxB);
inline f32 Math_Lerp(f32 a, f32 b, f32 t);
inline f32 Math_InvLerp(f32 x, f32 a, f32 b);

inline f32 Math_Sin(f32 t);
inline f32 Math_Asin(f32 t);
inline f32 Math_Cos(f32 t);
inline f32 Math_Acos(f32 t);
inline f32 Math_Tan(f32 t);
inline f32 Math_Atan(f32 t);
inline f32 Math_Atan2(f32 y, f32 x);

inline u32 Math_NextPowerOfTwo(u32 x);
inline u32 Math_Align(u32 x, u32 boundary);

inline bool Math_IsNearlyEqual(f32 a, f32 b);
inline bool Math_IsNearlyZero(f32 x);

inline f32 Math_Fmod(f32 a, f32 b)
{
	return fmodf(a, b);
}

inline f32 Math_Sqrt(f32 x)
{
	return sqrtf(x);
}

inline f32 Math_Pow(f32 x, f32 exp)
{
	return powf(x, exp);
}

inline f32 Math_Exp(f32 x)
{
	return expf(x);
}

inline f32 Math_Log(f32 x)
{
	return logf(x);
}

inline f32 Math_Log2(f32 x)
{
	return log2f(x);
}

inline f32 Math_Log10(f32 x)
{
	return log10f(x);
}

inline i32 Math_Floor(f32 x)
{
	return (i32)floorf(x);
}

inline f32 Math_Ffloor(f32 x)
{
	return floorf(x);
}

inline i32 Math_Round(f32 x)
{
	return (i32)roundf(x);
}

inline f32 Math_Fround(f32 x)
{
	return roundf(x);
}

inline i32 Math_Ceil(f32 x)
{
	return (i32)ceilf(x);
}

inline f32 Math_Fceil(f32 x)
{
	return ceilf(x);
}

inline i32 Math_Abs(i32 x)
{
	return x < 0 ? -x : x;
}

inline f32 Math_Fabs(f32 x)
{
	return x < 0.0f ? -x : x;
}

inline i32 Math_Sign(i32 x)
{
	return x < 0 ? -1 : 1;
}

inline f32 Math_Fsign(f32 x)
{
	return x < 0.0f ? -1.0f : 1.0f;
}

inline i32 Math_Min(i32 x, i32 y)
{
	return x < y ? x : y;
}

inline f32 Math_Fmin(f32 x, f32 y)
{
	return x < y ? x : y;
}

inline i32 Math_Max(i32 x, i32 y)
{
	return x > y ? x : y;
}

inline f32 Math_Fmax(f32 x, f32 y)
{
	return x > y ? x : y;
}

inline i32 Math_Clamp(i32 x, i32 min, i32 max)
{
	return x < min ? min : (x > max ? max : x);
}

inline f32 Math_Fclamp(f32 x, f32 min, f32 max)
{
	return x < min ? min : (x > max ? max : x);
}

inline f32 Math_Remap(f32 x, f32 minA, f32 maxA, f32 minB, f32 maxB)
{
	return (x - minA) / (maxA - minA) * (maxB - minB) + minB;
}

inline f32 Math_RemapClamped(f32 x, f32 minA, f32 maxA, f32 minB, f32 maxB)
{
	return Math_Fclamp((x - minA) / (maxA - minA), 0.0f, 1.0f) * (maxB - minB) + minB;
}

inline f32 Math_Lerp(f32 a, f32 b, f32 t)
{
	return a + (b - a) * t;
}

inline f32 Math_InvLerp(f32 x, f32 a, f32 b)
{
	return (x - a) / (b - a);
}

inline f32 Math_Sin(f32 t)
{
	return sinf(t);
}

inline f32 Math_Asin(f32 t)
{
	return asinf(t);
}

inline f32 Math_Cos(f32 t)
{
	return cosf(t);
}

inline f32 Math_Acos(f32 t)
{
	return acosf(t);
}

inline f32 Math_Tan(f32 t)
{
	return tan(t);
}

inline f32 Math_Atan(f32 t)
{
	return atan(t);
}

inline f32 Math_Atan2(f32 y, f32 x)
{
	return atan2(y, x);
}

inline u32 Math_NextPowerOfTwo(u32 x)
{
	--x;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	++x;
	return x;
}

inline u32 Math_Align(u32 x, u32 boundary)
{
	return (x + boundary - 1) & ~(boundary - 1);
}

inline bool Math_IsNearlyEqual(f32 a, f32 b)
{
	return Math_Fabs(b - a) <= MATH_EPSILON;
}

inline bool Math_IsNearlyZero(f32 x)
{
	return Math_Fabs(x) <= MATH_EPSILON;
}

#endif // HAS_MATH_H
