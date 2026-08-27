#ifndef HAS_MATHS_FUNCS_H
#define HAS_MATHS_FUNCS_H

#include "maths.h"

#include <math.h>

static inline f32 Math_Fmod(f32 a, f32 b);

static inline f32 Math_Sqrt(f32 x);
static inline f32 Math_Pow(f32 x, f32 exp);
static inline f32 Math_Exp(f32 x);
static inline f32 Math_Log(f32 x);
static inline f32 Math_Log2(f32 x);
static inline f32 Math_Log10(f32 x);

static inline i32 Math_Floor(f32 x);
static inline f32 Math_Ffloor(f32 x);
static inline i32 Math_Round(f32 x);
static inline f32 Math_Fround(f32 x);
static inline i32 Math_Ceil(f32 x);
static inline f32 Math_Fceil(f32 x);

static inline i32 Math_Abs(i32 x);
static inline f32 Math_Fabs(f32 x);
static inline i32 Math_Sign(i32 x);
static inline f32 Math_Fsign(f32 x);
static inline i32 Math_Min(i32 x, i32 y);
static inline f32 Math_Fmin(f32 x, f32 y);
static inline i32 Math_Max(i32 x, i32 y);
static inline f32 Math_Fmax(f32 x, f32 y);
static inline i32 Math_Clamp(i32 x, i32 min, i32 max);
static inline f32 Math_Fclamp(f32 x, f32 min, f32 max);
static inline f32 Math_Remap(f32 x, f32 minA, f32 maxA, f32 minB, f32 maxB);
static inline f32 Math_RemapClamped(f32 x, f32 minA, f32 maxA, f32 minB, f32 maxB);
static inline f32 Math_Lerp(f32 a, f32 b, f32 t);
static inline f32 Math_InvLerp(f32 x, f32 a, f32 b);

static inline f32 Math_Sin(f32 t);
static inline f32 Math_Asin(f32 t);
static inline f32 Math_Cos(f32 t);
static inline f32 Math_Acos(f32 t);
static inline f32 Math_Tan(f32 t);
static inline f32 Math_Atan(f32 t);
static inline f32 Math_Atan2(f32 y, f32 x);

static inline u32 Math_NextPowerOfTwo(u32 x);
static inline u32 Math_Align(u32 x, u32 boundary);

static inline bool Math_IsNearlyEqual(f32 a, f32 b);
static inline bool Math_IsNearlyZero(f32 x);

static inline float2 float2_Add(float2 a, float2 b);
static inline float2 float2_Sub(float2 a, float2 b);
static inline float2 float2_Mul(float2 a, f32 b);
static inline float2 float2_Div(float2 a, f32 b);
static inline f32 float2_Dot(float2 a, float2 b);

static inline f32 float2_LengthSq(float2 x);
static inline f32 float2_Length(float2 x);
static inline float2 float2_Normalized(float2 x);
static inline f32 float2_DistanceSq(float2 a, float2 b);
static inline f32 float2_Distance(float2 a, float2 b);

static inline float2 float2_FromAngle(f32 t);
static inline float2 float2_Rotated(float2 x, f32 t);
static inline f32 float2_Angle(float2 a, float2 b);
static inline f32 float2_AngleTo(float2 a, float2 b);
static inline float2 float2_Reflected(float2 x, float2 n);

static inline float2 float2_Min(float2 a, float2 b);
static inline float2 float2_Max(float2 a, float2 b);
static inline float2 float2_Clamp(float2 x, float2 min, float2 max);
static inline float2 float2_Lerp(float2 a, float2 b, f32 t);

static inline bool float2_IsNearlyEqual(float2 a, float2 b);
static inline bool float2_IsNearlyZero(float2 a);

static inline float3 float3_FromFloat2(float2 x, f32 z);

static inline float3 float3_Add(float3 a, float3 b);
static inline float3 float3_Sub(float3 a, float3 b);
static inline float3 float3_Mul(float3 a, f32 b);
static inline float3 float3_Div(float3 a, f32 b);
static inline f32 float3_Dot(float3 a, float3 b);
static inline float3 float3_Cross(float3 a, float3 b);

static inline f32 float3_LengthSq(float3 x);
static inline f32 float3_Length(float3 x);
static inline float3 float3_Normalized(float3 x);
static inline f32 float3_DistanceSq(float3 a, float3 b);
static inline f32 float3_Distance(float3 a, float3 b);

static inline float3 float3_Rotated(float3 x, float4 q);
static inline float3 float3_RotatedAxisAngle(float3 x, float3 axis, f32 t);
static inline f32 float3_AngleTo(float3 a, float3 b);
static inline float3 float3_Reflected(float3 x, float3 n);

static inline float3 float3_Min(float3 a, float3 b);
static inline float3 float3_Max(float3 a, float3 b);
static inline float3 float3_Clamp(float3 x, float3 min, float3 max);
static inline float3 float3_Lerp(float3 a, float3 b, f32 t);

static inline bool float3_IsNearlyEqual(float3 a, float3 b);
static inline bool float3_IsNearlyZero(float3 a);

static inline float4 float4_FromFloat2(float2 x, f32 z, f32 w);
static inline float4 float4_FromFloat3(float3 x, f32 w);

static inline float4 float4_Identity(void);
static inline float4 float4_AxisAngle(float3 axis, f32 t);

static inline float4 float4_Add(float4 a, float4 b);
static inline float4 float4_Sub(float4 a, float4 b);
static inline float4 float4_Mul(float4 a, f32 b);
static inline float4 float4_MulFloat4x4(float4 a, float4x4 b);
static inline float4 float4_Div(float4 a, f32 b);
static inline f32 float4_Dot(float4 a, float4 b);
static inline float4 float4_Combine(float4 a, float4 b);

static inline f32 float4_LengthSq(float4 x);
static inline f32 float4_Length(float4 x);
static inline float4 float4_Normalized(float4 x);
static inline f32 float4_DistanceSq(float4 a, float4 b);
static inline f32 float4_Distance(float4 a, float4 b);

static inline float4 float4_Min(float4 a, float4 b);
static inline float4 float4_Max(float4 a, float4 b);
static inline float4 float4_Clamp(float4 x, float4 min, float4 max);
static inline float4 float4_Lerp(float4 a, float4 b, f32 t);
static inline float4 float4_Slerp(float4 a, float4 b, f32 t);

static inline float4 float4_Conjugate(float4 x);
static inline float4 float4_Inverse(float4 x);

static inline bool float4_IsNearlyEqual(float4 a, float4 b);
static inline bool float4_IsNearlyZero(float4 a);

static inline float4x4 float4x4_Identity(f32 s);
static inline float4x4 float4x4_Orthographic(f32 right, f32 left, f32 top, f32 bottom, f32 near, f32 far);
static inline float4x4 float4x4_Perspective(f32 fov, f32 aspect, f32 near, f32 far);
static inline float4x4 float4x4_LookAt(float3 eyes, float3 target, float3 up);
static inline float4x4 float4x4_Translation(float3 to);
static inline float4x4 float4x4_Rotation(float4 q);
static inline float4x4 float4x4_RotationAxisAngle(float3 axis, f32 t);
static inline float4x4 float4x4_Scale(float3 scale);

static inline float4x4 float4x4_Add(float4x4 a, float4x4 b);
static inline float4x4 float4x4_Sub(float4x4 a, float4x4 b);
static inline float4x4 float4x4_Mul(float4x4 a, float4x4 b);
static inline float4x4 float4x4_MulScalar(float4x4 a, f32 b);
static inline float4 float4x4_MulFloat4(float4x4 a, float4 b);
static inline float4x4 float4x4_Transpose(float4x4 x);

static inline f32 float4x4_Determinant(float4x4 x);
static inline float4x4 float4x4_Inverse(float4x4 x);

static inline f32 Math_Fmod(f32 a, f32 b)
{
	return fmodf(a, b);
}

static inline f32 Math_Sqrt(f32 x)
{
	return sqrtf(x);
}

static inline f32 Math_Pow(f32 x, f32 exp)
{
	return powf(x, exp);
}

static inline f32 Math_Exp(f32 x)
{
	return expf(x);
}

static inline f32 Math_Log(f32 x)
{
	return logf(x);
}

static inline f32 Math_Log2(f32 x)
{
	return log2f(x);
}

static inline f32 Math_Log10(f32 x)
{
	return log10f(x);
}

static inline i32 Math_Floor(f32 x)
{
	return (i32)floorf(x);
}

static inline f32 Math_Ffloor(f32 x)
{
	return floorf(x);
}

static inline i32 Math_Round(f32 x)
{
	return (i32)roundf(x);
}

static inline f32 Math_Fround(f32 x)
{
	return roundf(x);
}

static inline i32 Math_Ceil(f32 x)
{
	return (i32)ceilf(x);
}

static inline f32 Math_Fceil(f32 x)
{
	return ceilf(x);
}

static inline i32 Math_Abs(i32 x)
{
	return x < 0 ? -x : x;
}

static inline f32 Math_Fabs(f32 x)
{
	return x < 0.0f ? -x : x;
}

static inline i32 Math_Sign(i32 x)
{
	return x < 0 ? -1 : 1;
}

static inline f32 Math_Fsign(f32 x)
{
	return x < 0.0f ? -1.0f : 1.0f;
}

static inline i32 Math_Min(i32 x, i32 y)
{
	return x < y ? x : y;
}

static inline f32 Math_Fmin(f32 x, f32 y)
{
	return x < y ? x : y;
}

static inline i32 Math_Max(i32 x, i32 y)
{
	return x > y ? x : y;
}

static inline f32 Math_Fmax(f32 x, f32 y)
{
	return x > y ? x : y;
}

static inline i32 Math_Clamp(i32 x, i32 min, i32 max)
{
	return x < min ? min : (x > max ? max : x);
}

static inline f32 Math_Fclamp(f32 x, f32 min, f32 max)
{
	return x < min ? min : (x > max ? max : x);
}

static inline f32 Math_Remap(f32 x, f32 minA, f32 maxA, f32 minB, f32 maxB)
{
	return (x - minA) / (maxA - minA) * (maxB - minB) + minB;
}

static inline f32 Math_RemapClamped(f32 x, f32 minA, f32 maxA, f32 minB, f32 maxB)
{
	return Math_Fclamp((x - minA) / (maxA - minA), 0.0f, 1.0f) * (maxB - minB) + minB;
}

static inline f32 Math_Lerp(f32 a, f32 b, f32 t)
{
	return a + (b - a) * t;
}

static inline f32 Math_InvLerp(f32 x, f32 a, f32 b)
{
	return (x - a) / (b - a);
}

static inline f32 Math_Sin(f32 t)
{
	return sinf(t);
}

static inline f32 Math_Asin(f32 t)
{
	return asinf(t);
}

static inline f32 Math_Cos(f32 t)
{
	return cosf(t);
}

static inline f32 Math_Acos(f32 t)
{
	return acosf(t);
}

static inline f32 Math_Tan(f32 t)
{
	return tan(t);
}

static inline f32 Math_Atan(f32 t)
{
	return atan(t);
}

static inline f32 Math_Atan2(f32 y, f32 x)
{
	return atan2(y, x);
}

static inline u32 Math_NextPowerOfTwo(u32 x)
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

static inline u32 Math_Align(u32 x, u32 boundary)
{
	return (x + boundary - 1) & ~(boundary - 1);
}

static inline bool Math_IsNearlyEqual(f32 a, f32 b)
{
	return Math_Fabs(b - a) <= MATH_EPSILON;
}

static inline bool Math_IsNearlyZero(f32 x)
{
	return Math_Fabs(x) <= MATH_EPSILON;
}

static inline float2 float2_Add(float2 a, float2 b)
{
	float2 result = { a.x + b.x, a.y + b.y };
	return result;
}

static inline float2 float2_Sub(float2 a, float2 b)
{
	float2 result = { a.x - b.x, a.y - b.y };
	return result;
}

static inline float2 float2_Mul(float2 a, f32 b)
{
	float2 result = { a.x * b, a.y * b };
	return result;
}

static inline float2 float2_Div(float2 a, f32 b)
{
	float2 result = { a.x / b, a.y / b };
	return result;
}

static inline f32 float2_Dot(float2 a, float2 b)
{
	return a.x * b.x + a.y * b.y;
}

static inline f32 float2_LengthSq(float2 x)
{
	return x.x * x.x + x.y * x.y;
}

static inline f32 float2_Length(float2 x)
{
	return Math_Sqrt(float2_LengthSq(x));
}

static inline float2 float2_Normalized(float2 x)
{
	f32 length = float2_LengthSq(x);
	float2 result = { 0 };
	if (length < MATH_EPSILON * MATH_EPSILON)
		return result;
	result.x = x.x / length;
	result.y = x.y / length;
	return result;
}

static inline f32 float2_DistanceSq(float2 a, float2 b)
{
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

static inline f32 float2_Distance(float2 a, float2 b)
{
	return Math_Sqrt(float2_DistanceSq(a, b));
}

static inline float2 float2_FromAngle(f32 t)
{
	float2 result = { Math_Cos(t), Math_Sin(t) };
	return result;
}

static inline float2 float2_Rotated(float2 x, f32 t)
{
	float2 result = {
		x.x * Math_Cos(t) - x.y * Math_Sin(t),
		x.x * Math_Sin(t) + x.y * Math_Cos(t)
	};
	return result;
}

static inline f32 float2_Angle(float2 a, float2 b)
{
	f32 aLength = float2_Length(a);
	f32 bLength = float2_Length(b);
	if (aLength == 0.0f || bLength == 0.0f)
		return 0.0f;
	f32 dot = float2_Dot(a, b);
	f32 cos = dot / (aLength * bLength);
	return Math_Acos(cos);
}

static inline f32 float2_AngleTo(float2 a, float2 b)
{
	float2 dir = float2_Sub(b, a);
	return Math_Atan2(dir.y, dir.x);
}

static inline float2 float2_Reflected(float2 x, float2 n)
{
	f32 dot = float2_Dot(x, n);
	float2 scaled = float2_Mul(n, dot * 2.0f);
	return float2_Sub(x, scaled);
}

static inline float2 float2_Min(float2 a, float2 b)
{
	float2 result = {
		Math_Fmin(a.x, b.x),
		Math_Fmin(a.y, b.y)
	};
	return result;
}

static inline float2 float2_Max(float2 a, float2 b)
{
	float2 result = {
		Math_Fmax(a.x, b.x),
		Math_Fmax(a.y, b.y)
	};
	return result;
}

static inline float2 float2_Clamp(float2 x, float2 min, float2 max)
{
	float2 result = {
		Math_Fclamp(x.x, min.x, max.x),
		Math_Fclamp(x.y, min.y, max.y)
	};
	return result;
}

static inline float2 float2_Lerp(float2 a, float2 b, f32 t)
{
	return float2_Add(a, float2_Mul(float2_Sub(b, a), t));
}

static inline bool float2_IsNearlyEqual(float2 a, float2 b)
{
	return Math_IsNearlyEqual(a.x, b.x) && Math_IsNearlyEqual(a.y, b.y);
}

static inline bool float2_IsNearlyZero(float2 a)
{
	return float2_LengthSq(a) <= MATH_EPSILON * MATH_EPSILON;
}

static inline float3 float3_FromFloat2(float2 x, f32 z)
{
	float3 result = { x.x, x.y, z };
	return result;
}

static inline float3 float3_Add(float3 a, float3 b)
{
	float3 result = { a.x + b.x, a.y + b.y, a.z + b.z };
	return result;
}

static inline float3 float3_Sub(float3 a, float3 b)
{
	float3 result = { a.x - b.x, a.y - b.y, a.z - b.z };
	return result;
}

static inline float3 float3_Mul(float3 a, f32 b)
{
	float3 result = { a.x * b, a.y * b, a.z * b };
	return result;
}

static inline float3 float3_Div(float3 a, f32 b)
{
	float3 result = { a.x / b, a.y / b, a.z / b };
	return result;
}

static inline f32 float3_Dot(float3 a, float3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline float3 float3_Cross(float3 a, float3 b)
{
	float3 result = {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.z * b.z,
		a.x * b.y - a.y - b.z
	};
	return result;
}

static inline f32 float3_LengthSq(float3 x)
{
	return x.x * x.x + x.y * x.y + x.z * x.z;
}

static inline f32 float3_Length(float3 x)
{
	return Math_Sqrt(float3_LengthSq(x));
}

static inline float3 float3_Normalized(float3 x)
{
	f32 length = float3_LengthSq(x);
	float3 result = { 0 };
	if (length < MATH_EPSILON * MATH_EPSILON)
		return result;
	result.x = x.x / length;
	result.y = x.y / length;
	return result;
}

static inline f32 float3_DistanceSq(float3 a, float3 b)
{
	return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z);
}

static inline f32 float3_Distance(float3 a, float3 b)
{
	return Math_Sqrt(float3_DistanceSq(a, b));
}

static inline float3 float3_Rotated(float3 x, float4 q)
{
	float3 u = { q.x, q.y, q.z };
	float3 ux = float3_Cross(u, x);
	float3 uux = float3_Cross(u, ux);

	float3 result = {
		x.x + 2.0f * (q.w * ux.x + uux.x),
		x.y + 2.0f * (q.w * ux.y + uux.y),
		x.z + 2.0f * (q.w * ux.z + uux.z),
	};
	return result;
}

static inline float3 float3_RotatedAxisAngle(float3 x, float3 axis, f32 t)
{
	f32 tCos = Math_Cos(t);
	f32 tSin = Math_Sin(t);
	f32 dot = float3_Dot(x, axis) * (1.0f - tCos);

	float3 result = { 0 };
	result.x = x.x * tCos + (axis.y * x.z - axis.z * x.y) * tSin + axis.x * dot;
	result.y = x.y * tCos + (axis.z * x.x - axis.x * x.z) * tSin + axis.y * dot;
	result.z = x.z * tCos + (axis.x * x.y - axis.y * x.x) * tSin + axis.z * dot;
	return result;
}

static inline f32 float3_AngleTo(float3 a, float3 b)
{
	f32 crossLength = float3_Length(float3_Cross(a, b));
	return Math_Atan2(crossLength, float3_Dot(a, b));
}

static inline float3 float3_Reflected(float3 x, float3 n)
{
	f32 dot = float3_Dot(x, n);
	float3 scaled = float3_Mul(n, dot * 2.0f);
	return float3_Sub(x, scaled);
}

static inline float3 float3_Min(float3 a, float3 b)
{
	float3 result = {
		Math_Fmin(a.x, b.x),
		Math_Fmin(a.y, b.y),
		Math_Fmin(a.z, b.z)
	};
	return result;
}

static inline float3 float3_Max(float3 a, float3 b)
{
	float3 result = {
		Math_Fmax(a.x, b.x),
		Math_Fmax(a.y, b.y),
		Math_Fmax(a.z, b.z)
	};
	return result;
}

static inline float3 float3_Clamp(float3 x, float3 min, float3 max)
{
	float3 result = {
		Math_Fclamp(x.x, min.x, max.x),
		Math_Fclamp(x.y, min.y, max.y),
		Math_Fclamp(x.z, min.z, max.z)
	};
	return result;
}

static inline float3 float3_Lerp(float3 a, float3 b, f32 t)
{
	return float3_Add(a, float3_Mul(float3_Sub(b, a), t));
}

static inline bool float3_IsNearlyEqual(float3 a, float3 b)
{
	return Math_IsNearlyEqual(a.x, b.x) && Math_IsNearlyEqual(a.y, b.y) && Math_IsNearlyEqual(a.z, b.z);
}

static inline bool float3_IsNearlyZero(float3 a)
{
	return float3_LengthSq(a) <= MATH_EPSILON * MATH_EPSILON;
}

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

static inline float4 float4_Identity(void)
{
	float4 result = { 0.0f, 0.0f, 0.0f, 1.0f };
	return result;
}

static inline float4 float4_AxisAngle(float3 axis, f32 t)
{
	f32 tSin = Math_Sin(t * 0.5f);

	float4 result = float4_Identity();
	result.x = axis.x * tSin;
	result.y = axis.y * tSin;
	result.z = axis.z * tSin;
	result.w = Math_Cos(t * 0.5f);
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

static inline float4 float4_MulFloat4x4(float4 a, float4x4 b)
{
	float4 result = { 0 };
	result.x = a.x * b.columns.i.x + a.y * b.columns.i.y + a.z * b.columns.i.z + a.w * b.columns.i.w;
	result.y = a.x * b.columns.j.x + a.y * b.columns.j.y + a.z * b.columns.j.z + a.w * b.columns.j.w;
	result.z = a.x * b.columns.k.x + a.y * b.columns.k.y + a.z * b.columns.k.z + a.w * b.columns.k.w;
	result.w = a.x * b.columns.l.x + a.y * b.columns.l.y + a.z * b.columns.l.z + a.w * b.columns.l.w;
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

static inline float4 float4_Combine(float4 a, float4 b)
{
	float3 xyzA = { a.x, a.y, a.z };
	float3 xyzB = { b.x, b.y, b.z };
	float3 ab = float3_Mul(xyzA, b.w);
	float3 ba = float3_Mul(xyzB, a.w);
	float3 im = float3_Add(float3_Add(ab, ba), float3_Cross(xyzA, xyzB));
	float4 result = {
		im.x, im.y, im.z,
		a.w * b.w - (a.x * b.x + a.y * b.y + a.z * b.z)
	};
	return result;
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

static inline float4 float4_Slerp(float4 a, float4 b, f32 t)
{
	f32 dot = float4_Dot(a, b);
	if (dot < 0.0f) {
		b = float4_Inverse(b);
		dot = float4_Dot(a, b);
	}
	f32 th = Math_Acos(dot);
	f32 thSinMul = 1.0f / Math_Sin(th);
	f32 fac0 = Math_Sin((1.0f - t) * th) * thSinMul;
	f32 fac1 = Math_Sin(t * th) * thSinMul;
	return float4_Add(float4_Mul(a, fac0), float4_Mul(b, fac1));
}

static inline float4 float4_Conjugate(float4 x)
{
	float4 result = { -x.x, -x.y, -x.z, x.w };
	return result;
}

static inline float4 float4_Inverse(float4 x)
{
	float4 result = float4_Div(float4_Conjugate(x), float4_LengthSq(x));
	return result;
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

static inline float4x4 float4x4_Identity(f32 s)
{
	float4x4 result = { 0 };
	result.columns.i.x = s;
	result.columns.j.y = s;
	result.columns.k.z = s;
	result.columns.l.w = s;
	return result;
}

static inline float4x4 float4x4_Orthographic(f32 right, f32 left, f32 top, f32 bottom, f32 near, f32 far)
{
	float4x4 result = float4x4_Identity(1.0f);
	result.columns.i.x = 2.0f / (right - left);
	result.columns.j.y = 2.0f / (top - bottom);
	result.columns.k.z = -2.0f / (far - near);
	result.columns.l.x = -(right + left) / (right - left);
	result.columns.l.y = -(top + bottom) / (top - bottom);
	result.columns.l.z = -(far + near) / (far - near);
	return result;
}

static inline float4x4 float4x4_Perspective(f32 fov, f32 aspect, f32 near, f32 far)
{
	f32 cot = 1.0f / Math_Tan(fov * 0.5f);

	float4x4 result = { 0 };
	result.columns.i.x = cot / aspect;
	result.columns.j.y = cot;
	result.columns.k.z = -(far + near) / (far - near);
	result.columns.k.w = -1.0f;
	result.columns.l.z = -(2.0f * far * near) / (far - near);
	return result;
}

static inline float4x4 float4x4_LookAt(float3 eyes, float3 target, float3 up)
{
	float3 k = float3_Normalized(float3_Sub(eyes, target));
	float3 i = float3_Normalized(float3_Cross(up, k));
	float3 j = float3_Cross(k, i);

	float4x4 result = float4x4_Identity(1.0f);

	result.columns.i.x = i.x;
	result.columns.i.y = i.y;
	result.columns.i.z = i.z;

	result.columns.j.x = j.x;
	result.columns.j.y = j.y;
	result.columns.j.z = j.z;

	result.columns.k.x = k.x;
	result.columns.k.y = k.y;
	result.columns.k.z = k.z;

	result.columns.l.x = -float3_Dot(i, eyes);
	result.columns.l.y = -float3_Dot(j, eyes);
	result.columns.l.z = -float3_Dot(k, eyes);

	return result;
}

static inline float4x4 float4x4_Translation(float3 to)
{
	float4x4 result = float4x4_Identity(1.0f);
	result.columns.l.x = to.x;
	result.columns.l.y = to.y;
	result.columns.l.z = to.z;
	return result;
}

static inline float4x4 float4x4_Rotation(float4 q)
{
	f32 xx2 = q.x * q.x * 2.0f;
	f32 yy2 = q.y * q.y * 2.0f;
	f32 zz2 = q.z * q.z * 2.0f;

	f32 xy2 = q.x * q.y * 2.0f;
	f32 xz2 = q.x * q.z * 2.0f;
	f32 yz2 = q.y * q.z * 2.0f;
	f32 wx2 = q.w * q.x * 2.0f;
	f32 wy2 = q.w * q.y * 2.0f;
	f32 wz2 = q.w * q.z * 2.0f;

	float4x4 result = float4x4_Identity(1.0f);
	result.columns.i.x = 1.0f - yy2 - zz2;
	result.columns.i.y = xy2 + wz2;
	result.columns.i.z = xz2 - wy2;
	result.columns.j.x = xy2 - wz2;
	result.columns.j.y = 1.0f - xx2 - zz2;
	result.columns.j.z = yz2 + wx2;
	result.columns.k.x = xz2 + wy2;
	result.columns.k.y = yz2 - wx2;
	result.columns.k.z = 1.0f - xx2 - yy2;
	return result;
}

static inline float4x4 float4x4_RotationAxisAngle(float3 axis, f32 t)
{
	f32 tCos = Math_Cos(t);
	f32 tSin = Math_Sin(t);

	float4x4 result = float4x4_Identity(1.0f);

	result.columns.i.x = tCos + axis.x * axis.x * (1.0f - tCos);
	result.columns.i.y = axis.z * tSin + axis.y * axis.x * (1.0f - tCos);
	result.columns.i.z = -axis.y * tSin + axis.z * axis.x * (1.0f - tCos);

	result.columns.j.x = -axis.z * tSin + axis.x * axis.y * (1.0f - tCos);
	result.columns.j.y = tCos + axis.y * axis.y * (1.0f - tCos);
	result.columns.j.z = axis.x * tSin + axis.z * axis.y * (1.0f - tCos);

	result.columns.k.x = axis.y * tSin + axis.x * axis.z * (1.0f - tCos);
	result.columns.k.y = -axis.x * tSin + axis.y * axis.z * (1.0f - tCos);
	result.columns.k.z = tCos + axis.z * axis.z * (1.0f - tCos);

	return result;
}

static inline float4x4 float4x4_Scale(float3 scale)
{
	float4x4 result = float4x4_Identity(1.0f);
	result.columns.i.x = scale.x;
	result.columns.j.y = scale.y;
	result.columns.k.z = scale.z;
	return result;
}

static inline float4x4 float4x4_Add(float4x4 a, float4x4 b)
{
	float4x4 result = { 0 };

	result.columns.i.x = a.columns.i.x + b.columns.i.x;
	result.columns.i.y = a.columns.i.y + b.columns.i.y;
	result.columns.i.z = a.columns.i.z + b.columns.i.z;
	result.columns.i.w = a.columns.i.w + b.columns.i.w;

	result.columns.j.x = a.columns.j.x + b.columns.j.x;
	result.columns.j.y = a.columns.j.y + b.columns.j.y;
	result.columns.j.z = a.columns.j.z + b.columns.j.z;
	result.columns.j.w = a.columns.j.w + b.columns.j.w;

	result.columns.k.x = a.columns.k.x + b.columns.k.x;
	result.columns.k.y = a.columns.k.y + b.columns.k.y;
	result.columns.k.z = a.columns.k.z + b.columns.k.z;
	result.columns.k.w = a.columns.k.w + b.columns.k.w;

	result.columns.l.x = a.columns.l.x + b.columns.l.x;
	result.columns.l.y = a.columns.l.y + b.columns.l.y;
	result.columns.l.z = a.columns.l.z + b.columns.l.z;
	result.columns.l.w = a.columns.l.w + b.columns.l.w;

	return result;
}

static inline float4x4 float4x4_Sub(float4x4 a, float4x4 b)
{
	float4x4 result = { 0 };

	result.columns.i.x = a.columns.i.x - b.columns.i.x;
	result.columns.i.y = a.columns.i.y - b.columns.i.y;
	result.columns.i.z = a.columns.i.z - b.columns.i.z;
	result.columns.i.w = a.columns.i.w - b.columns.i.w;

	result.columns.j.x = a.columns.j.x - b.columns.j.x;
	result.columns.j.y = a.columns.j.y - b.columns.j.y;
	result.columns.j.z = a.columns.j.z - b.columns.j.z;
	result.columns.j.w = a.columns.j.w - b.columns.j.w;

	result.columns.k.x = a.columns.k.x - b.columns.k.x;
	result.columns.k.y = a.columns.k.y - b.columns.k.y;
	result.columns.k.z = a.columns.k.z - b.columns.k.z;
	result.columns.k.w = a.columns.k.w - b.columns.k.w;

	result.columns.l.x = a.columns.l.x - b.columns.l.x;
	result.columns.l.y = a.columns.l.y - b.columns.l.y;
	result.columns.l.z = a.columns.l.z - b.columns.l.z;
	result.columns.l.w = a.columns.l.w - b.columns.l.w;

	return result;
}

static inline float4x4 float4x4_Mul(float4x4 a, float4x4 b)
{
	float4x4 result = { 0 };

	result.columns.i.x =
		a.columns.i.x * b.columns.i.x +
		a.columns.j.x * b.columns.i.y +
		a.columns.k.x * b.columns.i.z +
		a.columns.l.x * b.columns.i.w;
	result.columns.i.y =
		a.columns.i.y * b.columns.i.x +
		a.columns.j.y * b.columns.i.y +
		a.columns.k.y * b.columns.i.z +
		a.columns.l.y * b.columns.i.w;
	result.columns.i.z =
		a.columns.i.z * b.columns.i.x +
		a.columns.j.z * b.columns.i.y +
		a.columns.k.z * b.columns.i.z +
		a.columns.l.z * b.columns.i.w;
	result.columns.i.w =
		a.columns.i.w * b.columns.i.x +
		a.columns.j.w * b.columns.i.y +
		a.columns.k.w * b.columns.i.z +
		a.columns.l.w * b.columns.i.w;

	result.columns.j.x =
		a.columns.i.x * b.columns.j.x +
		a.columns.j.x * b.columns.j.y +
		a.columns.k.x * b.columns.j.z +
		a.columns.l.x * b.columns.j.w;
	result.columns.j.y =
		a.columns.i.y * b.columns.j.x +
		a.columns.j.y * b.columns.j.y +
		a.columns.k.y * b.columns.j.z +
		a.columns.l.y * b.columns.j.w;
	result.columns.j.z =
		a.columns.i.z * b.columns.j.x +
		a.columns.j.z * b.columns.j.y +
		a.columns.k.z * b.columns.j.z +
		a.columns.l.z * b.columns.j.w;
	result.columns.j.w =
		a.columns.i.w * b.columns.j.x +
		a.columns.j.w * b.columns.j.y +
		a.columns.k.w * b.columns.j.z +
		a.columns.l.w * b.columns.j.w;

	result.columns.k.x =
		a.columns.i.x * b.columns.k.x +
		a.columns.j.x * b.columns.k.y +
		a.columns.k.x * b.columns.k.z +
		a.columns.l.x * b.columns.k.w;
	result.columns.k.y =
		a.columns.i.y * b.columns.k.x +
		a.columns.j.y * b.columns.k.y +
		a.columns.k.y * b.columns.k.z +
		a.columns.l.y * b.columns.k.w;
	result.columns.k.z =
		a.columns.i.z * b.columns.k.x +
		a.columns.j.z * b.columns.k.y +
		a.columns.k.z * b.columns.k.z +
		a.columns.l.z * b.columns.k.w;
	result.columns.k.w =
		a.columns.i.w * b.columns.k.x +
		a.columns.j.w * b.columns.k.y +
		a.columns.k.w * b.columns.k.z +
		a.columns.l.w * b.columns.k.w;

	result.columns.l.x =
		a.columns.i.x * b.columns.l.x +
		a.columns.j.x * b.columns.l.y +
		a.columns.k.x * b.columns.l.z +
		a.columns.l.x * b.columns.l.w;
	result.columns.l.y =
		a.columns.i.y * b.columns.l.x +
		a.columns.j.y * b.columns.l.y +
		a.columns.k.y * b.columns.l.z +
		a.columns.l.y * b.columns.l.w;
	result.columns.l.z =
		a.columns.i.z * b.columns.l.x +
		a.columns.j.z * b.columns.l.y +
		a.columns.k.z * b.columns.l.z +
		a.columns.l.z * b.columns.l.w;
	result.columns.l.w =
		a.columns.i.w * b.columns.l.x +
		a.columns.j.w * b.columns.l.y +
		a.columns.k.w * b.columns.l.z +
		a.columns.l.w * b.columns.l.w;

	return result;
}

static inline float4x4 float4x4_MulScalar(float4x4 a, f32 b)
{
	float4x4 result = { 0 };

	result.columns.i.x = a.columns.i.x * b;
	result.columns.i.y = a.columns.i.y * b;
	result.columns.i.z = a.columns.i.z * b;
	result.columns.i.w = a.columns.i.w * b;

	result.columns.j.x = a.columns.j.x * b;
	result.columns.j.y = a.columns.j.y * b;
	result.columns.j.z = a.columns.j.z * b;
	result.columns.j.w = a.columns.j.w * b;

	result.columns.k.x = a.columns.k.x * b;
	result.columns.k.y = a.columns.k.y * b;
	result.columns.k.z = a.columns.k.z * b;
	result.columns.k.w = a.columns.k.w * b;

	result.columns.l.x = a.columns.l.x * b;
	result.columns.l.y = a.columns.l.y * b;
	result.columns.l.z = a.columns.l.z * b;
	result.columns.l.w = a.columns.l.w * b;

	return result;
}

static inline float4 float4x4_MulFloat4(float4x4 a, float4 b)
{
	float4 result = { 0 };
	result.x =
		a.columns.i.x * b.x +
		a.columns.j.x * b.y +
		a.columns.k.x * b.z +
		a.columns.l.x * b.w;
	result.y =
		a.columns.i.y * b.x +
		a.columns.j.y * b.y +
		a.columns.k.y * b.z +
		a.columns.l.y * b.w;
	result.z =
		a.columns.i.z * b.x +
		a.columns.j.z * b.y +
		a.columns.k.z * b.z +
		a.columns.l.z * b.w;
	result.w =
		a.columns.i.w * b.x +
		a.columns.j.w * b.y +
		a.columns.k.w * b.z +
		a.columns.l.w * b.w;
	return result;
}

static inline float4x4 float4x4_Transpose(float4x4 x)
{
	float4x4 result = { 0 };

	result.columns.i.y = x.columns.j.x;
	result.columns.i.z = x.columns.k.x;
	result.columns.i.w = x.columns.l.x;

	result.columns.j.x = x.columns.i.y;
	result.columns.j.z = x.columns.k.y;
	result.columns.j.w = x.columns.l.y;

	result.columns.k.x = x.columns.i.z;
	result.columns.k.y = x.columns.j.z;
	result.columns.k.w = x.columns.l.z;

	result.columns.l.x = x.columns.i.w;
	result.columns.l.y = x.columns.j.w;
	result.columns.l.z = x.columns.k.w;

	return result;
}

static inline f32 float4x4_Determinant(float4x4 x)
{
	f32 iZjW = x.columns.i.z * x.columns.j.w - x.columns.j.z * x.columns.i.w;
	f32 iZkW = x.columns.i.z * x.columns.k.w - x.columns.k.z * x.columns.i.w;
	f32 iZlW = x.columns.i.z * x.columns.l.w - x.columns.l.z * x.columns.i.w;
	f32 jZkW = x.columns.j.z * x.columns.k.w - x.columns.k.z * x.columns.j.w;
	f32 jZlW = x.columns.j.z * x.columns.l.w - x.columns.l.z * x.columns.j.w;
	f32 kZlW = x.columns.k.z * x.columns.l.w - x.columns.l.z * x.columns.k.w;

	f32 iDet = x.columns.i.x * (x.columns.j.y * kZlW - x.columns.k.y * jZlW + x.columns.l.y * jZkW);
	f32 jDet = -x.columns.j.x * (x.columns.i.y * kZlW - x.columns.k.y * iZlW + x.columns.l.y * iZkW);
	f32 kDet = x.columns.k.x * (x.columns.i.y * jZlW - x.columns.j.y * iZlW + x.columns.l.y * iZjW);
	f32 lDet = -x.columns.l.x * (x.columns.i.y * jZkW - x.columns.j.y * iZkW + x.columns.k.y * iZjW);
	return iDet + jDet + kDet + lDet;
}

static inline float4x4 float4x4_Inverse(float4x4 x)
{
	f32 iYjZ = x.columns.i.y * x.columns.j.z - x.columns.j.y * x.columns.i.z;
	f32 iYjW = x.columns.i.y * x.columns.j.w - x.columns.j.y * x.columns.i.w;
	f32 iYkZ = x.columns.i.y * x.columns.k.z - x.columns.k.y * x.columns.i.z;
	f32 iYkW = x.columns.i.y * x.columns.k.w - x.columns.k.y * x.columns.i.w;
	f32 iYlZ = x.columns.i.y * x.columns.l.z - x.columns.l.y * x.columns.i.z;
	f32 iYlW = x.columns.i.y * x.columns.l.w - x.columns.l.y * x.columns.i.w;
	f32 iZjW = x.columns.i.z * x.columns.j.w - x.columns.j.z * x.columns.i.w;
	f32 iZkW = x.columns.i.z * x.columns.k.w - x.columns.k.z * x.columns.i.w;
	f32 iZlW = x.columns.i.z * x.columns.l.w - x.columns.l.z * x.columns.i.w;
	f32 jYkZ = x.columns.j.y * x.columns.k.z - x.columns.k.y * x.columns.j.z;
	f32 jYkW = x.columns.j.y * x.columns.k.w - x.columns.k.y * x.columns.j.w;
	f32 jYlZ = x.columns.j.y * x.columns.l.z - x.columns.l.y * x.columns.j.z;
	f32 jYlW = x.columns.j.y * x.columns.l.w - x.columns.l.y * x.columns.j.w;
	f32 jZkW = x.columns.j.z * x.columns.k.w - x.columns.k.z * x.columns.j.w;
	f32 jZlW = x.columns.j.z * x.columns.l.w - x.columns.l.z * x.columns.j.w;
	f32 kYlZ = x.columns.k.y * x.columns.l.z - x.columns.l.y * x.columns.k.z;
	f32 kYlW = x.columns.k.y * x.columns.l.w - x.columns.l.y * x.columns.k.w;
	f32 kZlW = x.columns.k.z * x.columns.l.w - x.columns.l.z * x.columns.k.w;

	float4x4 result = { 0 };

	result.columns.i.x = x.columns.i.x * (x.columns.j.y * kZlW - x.columns.k.y * jZlW + x.columns.l.y * jZkW);
	result.columns.i.y = -x.columns.j.x * (x.columns.i.y * kZlW - x.columns.k.y * iZlW + x.columns.l.y * iZkW);
	result.columns.i.z = x.columns.k.x * (x.columns.i.y * jZlW - x.columns.j.y * iZlW + x.columns.l.y * iZjW);
	result.columns.i.w = -x.columns.l.x * (x.columns.i.y * jZkW - x.columns.j.y * iZkW + x.columns.k.y * iZjW);
	f32 det = result.columns.i.x + result.columns.i.y + result.columns.i.z + result.columns.i.w;
	if (Math_IsNearlyZero(det))
		return float4x4_Identity(1.0f);

	result.columns.j.x = -x.columns.i.y * (x.columns.j.x * kZlW - x.columns.k.x * jZlW + x.columns.l.x * jZkW);
	result.columns.j.y = x.columns.j.y * (x.columns.i.x * kZlW - x.columns.k.x * iZlW + x.columns.l.x * iZkW);
	result.columns.j.z = -x.columns.k.y * (x.columns.i.x * jZlW - x.columns.j.x * iZlW + x.columns.l.x * iZjW);
	result.columns.j.w = x.columns.l.y * (x.columns.i.x * jZkW - x.columns.j.x * iZkW + x.columns.k.x * iZjW);

	result.columns.k.x = x.columns.i.z * (x.columns.j.x * kYlW - x.columns.k.x * jYlW + x.columns.l.x * jYkW);
	result.columns.k.y = -x.columns.j.z * (x.columns.i.x * kYlW - x.columns.k.x * iYlW + x.columns.l.x * iYkW);
	result.columns.k.z = x.columns.k.z * (x.columns.i.x * jYlW - x.columns.j.x * iYlW + x.columns.l.x * iYjW);
	result.columns.k.w = -x.columns.l.z * (x.columns.i.x * jYkW - x.columns.j.x * iYkW + x.columns.k.x * iYjW);

	result.columns.l.x = -x.columns.i.w * (x.columns.j.x * kYlZ - x.columns.k.x * jYlZ + x.columns.l.x * jYkZ);
	result.columns.l.y = x.columns.j.w * (x.columns.i.x * kYlZ - x.columns.k.x * iYlZ + x.columns.l.x * iYkZ);
	result.columns.l.z = -x.columns.k.w * (x.columns.i.x * jYlZ - x.columns.j.x * iYlZ + x.columns.l.x * iYjZ);
	result.columns.l.w = x.columns.l.w * (x.columns.i.x * jYkZ - x.columns.j.x * iYkZ + x.columns.k.x * iYjZ);

	f32 detMul = 1.0f / det;
	result.columns.i.x *= detMul;
	result.columns.i.y *= detMul;
	result.columns.i.z *= detMul;
	result.columns.i.w *= detMul;
	result.columns.j.x *= detMul;
	result.columns.j.y *= detMul;
	result.columns.j.z *= detMul;
	result.columns.j.w *= detMul;
	result.columns.k.x *= detMul;
	result.columns.k.y *= detMul;
	result.columns.k.z *= detMul;
	result.columns.k.w *= detMul;
	result.columns.l.x *= detMul;
	result.columns.l.y *= detMul;
	result.columns.l.z *= detMul;
	result.columns.l.w *= detMul;

	return result;
}

#endif // HAS_MATHS_FUNCS_H
