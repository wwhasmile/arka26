#include "float3.h"

extern inline float3 float3_FromFloat2(float2 x, f32 z);

extern inline float3 float3_Add(float3 a, float3 b);
extern inline float3 float3_Sub(float3 a, float3 b);
extern inline float3 float3_Mul(float3 a, f32 b);
extern inline float3 float3_Div(float3 a, f32 b);
extern inline f32 float3_Dot(float3 a, float3 b);
extern inline float3 float3_Cross(float3 a, float3 b);

extern inline f32 float3_LengthSq(float3 x);
extern inline f32 float3_Length(float3 x);
extern inline float3 float3_Normalized(float3 x);
extern inline f32 float3_DistanceSq(float3 a, float3 b);
extern inline f32 float3_Distance(float3 a, float3 b);

extern inline f32 float3_AngleTo(float3 a, float3 b);
extern inline float3 float3_Reflected(float3 x, float3 n);

extern inline float3 float3_Min(float3 a, float3 b);
extern inline float3 float3_Max(float3 a, float3 b);
extern inline float3 float3_Clamp(float3 x, float3 min, float3 max);
extern inline float3 float3_Lerp(float3 a, float3 b, f32 t);

extern inline bool float3_IsNearlyEqual(float3 a, float3 b);
extern inline bool float3_IsNearlyZero(float3 a);
