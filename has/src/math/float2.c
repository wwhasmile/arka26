#include "float2.h"

extern inline float2 float2_Add(float2 a, float2 b);
extern inline float2 float2_Sub(float2 a, float2 b);
extern inline float2 float2_Mul(float2 a, f32 b);
extern inline float2 float2_Div(float2 a, f32 b);
extern inline f32 float2_Dot(float2 a, float2 b);

extern inline f32 float2_LengthSq(float2 x);
extern inline f32 float2_Length(float2 x);
extern inline float2 float2_Normalized(float2 x);
extern inline f32 float2_DistanceSq(float2 a, float2 b);
extern inline f32 float2_Distance(float2 a, float2 b);

extern inline float2 float2_FromAngle(f32 t);
extern inline float2 float2_Rotated(float2 x, f32 t);
extern inline f32 float2_Angle(float2 a, float2 b);
extern inline f32 float2_AngleTo(float2 a, float2 b);
extern inline float2 float2_Reflected(float2 x, float2 n);

extern inline float2 float2_Min(float2 a, float2 b);
extern inline float2 float2_Max(float2 a, float2 b);
extern inline float2 float2_Clamp(float2 x, float2 min, float2 max);
extern inline float2 float2_Lerp(float2 a, float2 b, f32 t);

extern inline bool float2_IsNearlyEqual(float2 a, float2 b);
extern inline bool float2_IsNearlyZero(float2 a);
