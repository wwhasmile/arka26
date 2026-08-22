#ifndef HAS_FLOAT4_H
#define HAS_FLOAT4_H

#include <math/math.h>
#include <math/float2.h>
#include <math/float3.h>

typedef struct {
	f32 x;
	f32 y;
	f32 z;
	f32 w;
} float4;

static inline float4 float4_FromFloat2(float2 x, f32 z, f32 w)
{
	float4 result = { x.x, x.y, z, w };
	return result;
}

static inline float4 float4_FromFloat3(float3 x, f32 w)
{
	float4 result = { x.x, x.y, x.z, w };
	return result;
}

static inline float4 float4_Add(float4 a, float4 b)
{
	float4 result = { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
	return result;
}

static inline float4 float4_Sub(float4 a, float4 b)
{
	float4 result = { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
	return result;
}

static inline float4 float4_Mul(float4 a, f32 b)
{
	float4 result = { a.x * b, a.y * b, a.z * b, a.w * b };
	return result;
}

static inline float4 float4_Div(float4 a, f32 b)
{
	float4 result = { a.x / b, a.y / b, a.z / b, a.w / b };
	return result;
}

static inline f32 float4_Dot(float4 a, float4 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

static inline f32 float4_LengthSq(float4 x)
{
	return x.x * x.x + x.y * x.y + x.z * x.z + x.w * x.w;
}

static inline f32 float4_Length(float4 x)
{
	return Math_Sqrt(float4_LengthSq(x));
}

static inline float4 float4_Normalized(float4 x)
{
	f32 length = float4_LengthSq(x);
	float4 result = { 0 };
	if (length < MATH_EPSILON * MATH_EPSILON)
		return result;
	result.x = x.x / length;
	result.y = x.y / length;
	return result;
}

static inline f32 float4_DistanceSq(float4 a, float4 b)
{
	return (b.x - a.x) * (b.x - a.x) +
		(b.y - a.y) * (b.y - a.y) +
		(b.z - a.z) * (b.z - a.z) +
		(b.w - a.w) * (b.w - a.w);
}

static inline f32 float4_Distance(float4 a, float4 b)
{
	return Math_Sqrt(float4_DistanceSq(a, b));
}

static inline float4 float4_Min(float4 a, float4 b)
{
	float4 result = {
		Math_Fmin(a.x, b.x),
		Math_Fmin(a.y, b.y),
		Math_Fmin(a.z, b.z),
		Math_Fmin(a.w, b.w)
	};
	return result;
}

static inline float4 float4_Max(float4 a, float4 b)
{
	float4 result = {
		Math_Fmax(a.x, b.x),
		Math_Fmax(a.y, b.y),
		Math_Fmax(a.z, b.z),
		Math_Fmax(a.w, b.w)
	};
	return result;
}

static inline float4 float4_Clamp(float4 x, float4 min, float4 max)
{
	float4 result = {
		Math_Fclamp(x.x, min.x, max.x),
		Math_Fclamp(x.y, min.y, max.y),
		Math_Fclamp(x.z, min.z, max.z),
		Math_Fclamp(x.w, min.w, max.w)
	};
	return result;
}

static inline float4 float4_Lerp(float4 a, float4 b, f32 t)
{
	return float4_Add(a, float4_Mul(float4_Sub(b, a), t));
}

static inline bool float4_IsNearlyEqual(float4 a, float4 b)
{
	return Math_IsNearlyEqual(a.x, b.x) &&
		Math_IsNearlyEqual(a.y, b.y) &&
		Math_IsNearlyEqual(a.z, b.z) &&
		Math_IsNearlyEqual(a.w, b.w);
}

static inline bool float4_IsNearlyZero(float4 a)
{
	return float4_LengthSq(a) <= MATH_EPSILON * MATH_EPSILON;
}

#endif // HAS_FLOAT4_H
