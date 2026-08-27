#ifndef HAS_UTIL_TWEENER_H
#define HAS_UTIL_TWEENER_H

#include <maths/maths.h>
#include <maths/color.h>
#include <maths/interpolation.h>

typedef struct {
    bool active;
    f32 duration;
    f32 t;
    Interpolation interp;
} TweenerState;

typedef struct {
    TweenerState state;
    f32 a;
    f32 b;
} f32Tweener;

typedef struct {
    TweenerState state;
    float2 a;
    float2 b;
} float2Tweener;

typedef struct {
    TweenerState state;
    float3 a;
    float3 b;
} float3Tweener;

typedef struct {
    TweenerState state;
    float4 a;
    float4 b;
} float4Tweener;

typedef struct {
    TweenerState state;
    Color8 a;
    Color8 b;
} Color8Tweener;

static inline void TweenerState_Start(TweenerState *state, f32 duration, Interpolation interp);
static inline f32 TweenerState_Update(TweenerState *state, f32 delta);

void f32Tweener_Start(f32Tweener *tweener, f32 a, f32 b, f32 duration, Interpolation interp);
f32 f32Tweener_Update(f32Tweener *tweener, f32 delta);

void float2Tweener_Start(float2Tweener *tweener, float2 a, float2 b, f32 duration, Interpolation interp);
float2 float2Tweener_Update(float2Tweener *tweener, f32 delta);

void float3Tweener_Start(float3Tweener *tweener, float3 a, float3 b, f32 duration, Interpolation interp);
float3 float3Tweener_Update(float3Tweener *tweener, f32 delta);

void float4Tweener_Start(float4Tweener *tweener, float4 a, float4 b, f32 duration, Interpolation interp);
float4 float4Tweener_Update(float4Tweener *tweener, f32 delta);
float4 float4Tweener_SlerpUpdate(float4Tweener *tweener, f32 delta);

void Color8Tweener_Start(Color8Tweener *tweener, Color8 a, Color8 b, f32 duration, Interpolation interp);
Color8 Color8Tweener_Update(Color8Tweener *tweener, f32 delta);

static inline void TweenerState_Start(TweenerState *state, f32 duration, Interpolation interp)
{
    state->duration = duration;
    state->t = 0.0f;
    state->interp = interp;
    state->active = true;
}

static inline f32 TweenerState_Update(TweenerState *state, f32 delta)
{
    if (!state->active)
        return state->t / state->duration;
    state->t += delta;
    if (state->t >= state->duration) {
        state->t = state->duration;
        state->active = false;
    }
    return state->t / state->duration;
}

#endif // HAS_UTIL_TWEENER_H