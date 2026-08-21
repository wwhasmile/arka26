#ifndef HAS_FLOAT4x4
#define HAS_FLOAT4x4

#include <math/math.h>
#include <math/float3.h>
#include <math/float4.h>

typedef union {
	f32 data[16];
	struct {
		float4 i;
		float4 j;
		float4 k;
		float4 l;
	} columns;
} float4x4;

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

static inline float4x4 float4x4_Rotation(float3 axis, f32 t)
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

#endif // HAS_FLOAT4x4
