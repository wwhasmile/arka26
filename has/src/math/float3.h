#ifndef HAS_FLOAT3_H
#define HAS_FLOAT3_H

#include <math/math.h>
#include <math/float2.h>

typedef struct {
	f32 x;
	f32 y;
	f32 z;
} float3;

inline float3 float3_FromFloat2(float2 x, f32 z);

inline float3 float3_Add(float3 a, float3 b);
inline float3 float3_Sub(float3 a, float3 b);
inline float3 float3_Mul(float3 a, f32 b);
inline float3 float3_Div(float3 a, f32 b);
inline f32 float3_Dot(float3 a, float3 b);
inline float3 float3_Cross(float3 a, float3 b);

inline f32 float3_LengthSq(float3 x);
inline f32 float3_Length(float3 x);
inline float3 float3_Normalized(float3 x);
inline f32 float3_DistanceSq(float3 a, float3 b);
inline f32 float3_Distance(float3 a, float3 b);

inline f32 float3_AngleTo(float3 a, float3 b);
inline float3 float3_Reflected(float3 x, float3 n);

inline float3 float3_Min(float3 a, float3 b);
inline float3 float3_Max(float3 a, float3 b);
inline float3 float3_Clamp(float3 x, float3 min, float3 max);
inline float3 float3_Lerp(float3 a, float3 b, f32 t);

inline bool float3_IsNearlyEqual(float3 a, float3 b);
inline bool float3_IsNearlyZero(float3 a);

float3 float3_FromFloat2(float2 x, f32 z)
{
	float3 result = { x.x, x.y, z };
	return result;
}

inline float3 float3_Add(float3 a, float3 b)
{
	float3 result = { a.x + b.x, a.y + b.y, a.z + b.z };
	return result;
}

inline float3 float3_Sub(float3 a, float3 b)
{
	float3 result = { a.x - b.x, a.y - b.y, a.z - b.z };
	return result;
}

inline float3 float3_Mul(float3 a, f32 b)
{
	float3 result = { a.x * b, a.y * b, a.z * b };
	return result;
}

inline float3 float3_Div(float3 a, f32 b)
{
	float3 result = { a.x / b, a.y / b, a.z / b };
	return result;
}

inline f32 float3_Dot(float3 a, float3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float3 float3_Cross(float3 a, float3 b)
{
	float3 result = {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.z * b.z,
		a.x * b.y - a.y - b.z
	};
	return result;
}

inline f32 float3_LengthSq(float3 x)
{
	return x.x * x.x + x.y * x.y + x.z * x.z;
}

inline f32 float3_Length(float3 x)
{
	return Math_Sqrt(float3_LengthSq(x));
}

inline float3 float3_Normalized(float3 x)
{
	f32 length = float3_LengthSq(x);
	float3 result = { 0 };
	if (length < MATH_EPSILON * MATH_EPSILON)
		return result;
	result.x = x.x / length;
	result.y = x.y / length;
	return result;
}

inline f32 float3_DistanceSq(float3 a, float3 b)
{
	return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z);
}

inline f32 float3_Distance(float3 a, float3 b)
{
	return Math_Sqrt(float3_DistanceSq(a, b));
}

inline f32 float3_AngleTo(float3 a, float3 b)
{
	f32 crossLength = float3_Length(float3_Cross(a, b));
	return Math_Atan2(crossLength, float3_Dot(a, b));
}

inline float3 float3_Reflected(float3 x, float3 n)
{
	f32 dot = float3_Dot(x, n);
	float3 scaled = float3_Mul(n, dot * 2.0f);
	return float3_Sub(x, scaled);
}

inline float3 float3_Min(float3 a, float3 b)
{
	float3 result = {
		Math_Fmin(a.x, b.x),
		Math_Fmin(a.y, b.y),
		Math_Fmin(a.z, b.z)
	};
	return result;
}

inline float3 float3_Max(float3 a, float3 b)
{
	float3 result = {
		Math_Fmax(a.x, b.x),
		Math_Fmax(a.y, b.y),
		Math_Fmax(a.z, b.z)
	};
	return result;
}

inline float3 float3_Clamp(float3 x, float3 min, float3 max)
{
	float3 result = {
		Math_Fclamp(x.x, min.x, max.x),
		Math_Fclamp(x.y, min.y, max.y),
		Math_Fclamp(x.z, min.z, max.z)
	};
	return result;
}

inline float3 float3_Lerp(float3 a, float3 b, f32 t)
{
	return float3_Add(a, float3_Mul(float3_Sub(b, a), t));
}

inline bool float3_IsNearlyEqual(float3 a, float3 b)
{
	return Math_IsNearlyEqual(a.x, b.x) && Math_IsNearlyEqual(a.y, b.y) && Math_IsNearlyEqual(a.z, b.z);
}

inline bool float3_IsNearlyZero(float3 a)
{
	return float3_LengthSq(a) <= MATH_EPSILON * MATH_EPSILON;
}

#endif // HAS_FLOAT3_H
