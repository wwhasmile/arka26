#include "tweener.h"

void f32Tweener_Start(f32Tweener *tweener, f32 a, f32 b, f32 duration, Interpolation interp)
{
	tweener->a = a;
	tweener->b = b;
	TweenerState_Start((TweenerState*)tweener, duration, interp);
}

f32 f32Tweener_Update(f32Tweener *tweener, f32 delta)
{
	f32 t = TweenerState_Update((TweenerState*)tweener, delta);
	return Math_Lerp(tweener->a, tweener->b, t);
}

void float2Tweener_Start(float2Tweener *tweener, float2 a, float2 b, f32 duration, Interpolation interp)
{
	tweener->a = a;
	tweener->b = b;
	TweenerState_Start((TweenerState*)tweener, duration, interp);
}

float2 float2Tweener_Update(float2Tweener *tweener, f32 delta)
{
	f32 t = TweenerState_Update((TweenerState*)tweener, delta);
	return float2_Lerp(tweener->a, tweener->b, t);
}

void float3Tweener_Start(float3Tweener *tweener, float3 a, float3 b, f32 duration, Interpolation interp)
{
	tweener->a = a;
	tweener->b = b;
	TweenerState_Start((TweenerState*)tweener, duration, interp);
}

float3 float3Tweener_Update(float3Tweener *tweener, f32 delta)
{
	f32 t = TweenerState_Update((TweenerState*)tweener, delta);
	return float3_Lerp(tweener->a, tweener->b, t);
}

void float4Tweener_Start(float4Tweener *tweener, float4 a, float4 b, f32 duration, Interpolation interp)
{
	tweener->a = a;
	tweener->b = b;
	TweenerState_Start((TweenerState*)tweener, duration, interp);
}

float4 float4Tweener_Update(float4Tweener *tweener, f32 delta)
{
	f32 t = TweenerState_Update((TweenerState*)tweener, delta);
	return float4_Lerp(tweener->a, tweener->b, t);
}

float4 float4Tweener_SlerpUpdate(float4Tweener *tweener, f32 delta)
{
    	f32 t = TweenerState_Update((TweenerState*)tweener, delta);
    	return float4_Slerp(tweener->a, tweener->b, t);
}

void Color8Tweener_Start(Color8Tweener *tweener, Color8 a, Color8 b, f32 duration, Interpolation interp)
{
    	tweener->a = a;
    	tweener->b = b;
    	TweenerState_Start((TweenerState*)tweener, duration, interp);
}

Color8 Color8Tweener_Update(Color8Tweener *tweener, f32 delta)
{
    	f32 t = TweenerState_Update((TweenerState*)tweener, delta);
    	return Color8_Lerp(tweener->a, tweener->b, t);
}
