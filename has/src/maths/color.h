#ifndef HAS_MATHS_COLOR_H
#define HAS_MATHS_COLOR_H

#include <maths/maths.h>

typedef union {
    u32 data;
    struct {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } rgba;
} Color8;

static inline Color8 Color8_FromRGBA(u32 rgba);
static inline Color8 Color8_FromComponents(u8 r, u8 g, u8 b, u8 a);
static inline Color8 Color8_FromFloat4(float4 x);
static inline float4 Color8_ToFloat4(Color8 x);

static inline Color8 Color8_Lerp(Color8 a, Color8 b, f32 t);

static inline Color8 Color8_FromRGBA(u32 rgba)
{
    Color8 result = { rgba };
    return result;
}

static inline Color8 Color8_FromComponents(u8 r, u8 g, u8 b, u8 a)
{
    Color8 result = { 0 };
    result.rgba.r = r;
    result.rgba.g = g;
    result.rgba.b = b;
    result.rgba.a = a;
    return result;
}

static inline Color8 Color8_FromFloat4(float4 x)
{
    Color8 result = { 0 };
    result.rgba.r = (u8)(x.x * 255.0f);
    result.rgba.g = (u8)(x.y * 255.0f);
    result.rgba.b = (u8)(x.z * 255.0f);
    result.rgba.a = (u8)(x.w * 255.0f);
    return result;
}

static inline float4 Color8_ToFloat4(Color8 x)
{
    f32 mul = 1.0f / 255.0f;
    float4 result = {
        x.rgba.r * mul,
        x.rgba.g * mul,
        x.rgba.b * mul,
        x.rgba.a * mul
    };
    return result;
}

static inline Color8 Color8_Lerp(Color8 a, Color8 b, f32 t)
{
    float4 aFloat4 = Color8_ToFloat4(a);
    float4 bFloat4 = Color8_ToFloat4(b);
    aFloat4.x += (bFloat4.x - aFloat4.x) * t;
    aFloat4.y += (bFloat4.y - aFloat4.y) * t;
    aFloat4.z += (bFloat4.z - aFloat4.z) * t;
    aFloat4.w += (bFloat4.w - aFloat4.w) * t;
    return Color8_FromFloat4(aFloat4);
}

#endif // HAS_MATHS_COLOR_H