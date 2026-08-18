#include "math.h"

extern inline f32 Math_Fmod(f32 a, f32 b);

extern inline f32 Math_Sqrt(f32 x);
extern inline f32 Math_Pow(f32 x, f32 exp);
extern inline f32 Math_Exp(f32 x);
extern inline f32 Math_Log(f32 x);
extern inline f32 Math_Log2(f32 x);
extern inline f32 Math_Log10(f32 x);

extern inline i32 Math_Floor(f32 x);
extern inline f32 Math_Ffloor(f32 x);
extern inline i32 Math_Round(f32 x);
extern inline f32 Math_Fround(f32 x);
extern inline i32 Math_Ceil(f32 x);
extern inline f32 Math_Fceil(f32 x);

extern inline i32 Math_Abs(i32 x);
extern inline f32 Math_Fabs(f32 x);
extern inline i32 Math_Sign(i32 x);
extern inline f32 Math_Fsign(f32 x);
extern inline i32 Math_Min(i32 x, i32 y);
extern inline f32 Math_Fmin(f32 x, f32 y);
extern inline i32 Math_Max(i32 x, i32 y);
extern inline f32 Math_Fmax(f32 x, f32 y);
extern inline i32 Math_Clamp(i32 x, i32 min, i32 max);
extern inline f32 Math_Fclamp(f32 x, f32 min, f32 max);
extern inline f32 Math_Remap(f32 x, f32 minA, f32 maxA, f32 minB, f32 maxB);
extern inline f32 Math_RemapClamped(f32 x, f32 minA, f32 maxA, f32 minB, f32 maxB);
extern inline f32 Math_Lerp(f32 a, f32 b, f32 t);
extern inline f32 Math_InvLerp(f32 x, f32 a, f32 b);

extern inline f32 Math_Sin(f32 t);
extern inline f32 Math_Asin(f32 t);
extern inline f32 Math_Cos(f32 t);
extern inline f32 Math_Acos(f32 t);
extern inline f32 Math_Tan(f32 t);
extern inline f32 Math_Atan(f32 t);
extern inline f32 Math_Atan2(f32 y, f32 x);

extern inline u32 Math_NextPowerOfTwo(u32 x);
extern inline u32 Math_Align(u32 x, u32 boundary);

extern inline bool Math_IsNearlyEqual(f32 a, f32 b);
extern inline bool Math_IsNearlyZero(f32 x);
