#ifndef HAS_FLOAT2_H
#define HAS_FLOAT2_H

#include <math/math.h>

typedef struct {
	f32 x;
	f32 y;
} float2;

inline float2 float2_Add(float2 a, float2 b);
inline float2 float2_Sub(float2 a, float2 b);
inline float2 float2_Mul(float2 a, f32 b);
inline float2 float2_Div(float2 a, f32 b);
inline f32 float2_Dot(float2 a, float2 b);

inline f32 float2_LengthSq(float2 x);
inline f32 float2_Length(float2 x);
inline float2 float2_Normalized(float2 x);
inline f32 float2_DistanceSq(float2 a, float2 b);
inline f32 float2_Distance(float2 a, float2 b);

inline float2 float2_FromAngle(f32 t);
inline float2 float2_Rotated(float2 x, f32 t);
inline f32 float2_Angle(float2 a, float2 b);
inline f32 float2_AngleTo(float2 a, float2 b);
inline float2 float2_Reflected(float2 x, float2 n);

inline float2 float2_Min(float2 a, float2 b);
inline float2 float2_Max(float2 a, float2 b);
inline float2 float2_Clamp(float2 x, float2 min, float2 max);
inline float2 float2_Lerp(float2 a, float2 b, f32 t);

inline bool float2_IsNearlyEqual(float2 a, float2 b);
inline bool float2_IsNearlyZero(float2 a);

inline float2 float2_Add(float2 a, float2 b)
{
	float2 result = { a.x + b.x, a.y + b.y };
	return result;
}

inline float2 float2_Sub(float2 a, float2 b)
{
	float2 result = { a.x - b.x, a.y - b.y };
	return result;
}

inline float2 float2_Mul(float2 a, f32 b)
{
	float2 result = { a.x * b, a.y * b };
	return result;
}

inline float2 float2_Div(float2 a, f32 b)
{
	float2 result = { a.x / b, a.y / b };
	return result;
}

inline f32 float2_Dot(float2 a, float2 b)
{
	return a.x * b.x + a.y * b.y;
}

inline f32 float2_LengthSq(float2 x)
{
	return x.x * x.x + x.y * x.y;
}

inline f32 float2_Length(float2 x)
{
	return Math_Sqrt(float2_LengthSq(x));
}

inline float2 float2_Normalized(float2 x)
{
	f32 length = float2_Length(x);
	x.x /= length;
	x.y /= length;
	return x;
}

inline f32 float2_DistanceSq(float2 a, float2 b)
{
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

inline f32 float2_Distance(float2 a, float2 b)
{
	return Math_Sqrt(float2_DistanceSq(a, b));
}

inline float2 float2_FromAngle(f32 t)
{
	float2 result = { Math_Cos(t), Math_Sin(t) };
	return result;
}

inline float2 float2_Rotated(float2 x, f32 t)
{
	float2 result = {
		x.x * Math_Cos(t) - x.y * Math_Sin(t),
		x.x * Math_Sin(t) + x.y * Math_Cos(t)
	};
	return result;
}

inline f32 float2_Angle(float2 a, float2 b)
{
	f32 aLength = float2_Length(a);
	f32 bLength = float2_Length(b);
	if (aLength == 0.0f || bLength == 0.0f)
		return 0.0f;
	f32 dot = float2_Dot(a, b);
	f32 cos = dot / (aLength * bLength);
	return Math_Acos(cos);
}

inline f32 float2_AngleTo(float2 a, float2 b)
{
	float2 dir = float2_Sub(b, a);
	return Math_Atan2(dir.y, dir.x);
}

inline float2 float2_Reflected(float2 x, float2 n)
{
	f32 dot = float2_Dot(x, n);
	float2 scaled = float2_Mul(n, dot * 2.0f);
	return float2_Sub(x, scaled);
}

inline float2 float2_Min(float2 a, float2 b)
{
	float2 result = {
		Math_Fmin(a.x, b.x),
		Math_Fmin(a.y, b.y)
	};
	return result;
}

inline float2 float2_Max(float2 a, float2 b)
{
	float2 result = {
		Math_Fmax(a.x, b.x),
		Math_Fmax(a.y, b.y)
	};
	return result;
}

inline float2 float2_Clamp(float2 x, float2 min, float2 max)
{
	float2 result = {
		Math_Fclamp(x.x, min.x, max.x),
		Math_Fclamp(x.y, min.y, max.y)
	};
	return result;
}

inline float2 float2_Lerp(float2 a, float2 b, f32 t)
{
	return float2_Add(a, float2_Mul(float2_Sub(b, a), t));
}

inline bool float2_IsNearlyEqual(float2 a, float2 b)
{
	return Math_IsNearlyEqual(a.x, b.x) && Math_IsNearlyEqual(a.y, b.y);
}

inline bool float2_IsNearlyZero(float2 a)
{
	return float2_LengthSq(a) <= MATH_EPSILON * MATH_EPSILON;
}

#endif // HAS_FLOAT2_H
