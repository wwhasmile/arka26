#ifndef ARKA26_GFX_H
#define ARKA26_GFX_H

#include <defines.h>

#ifndef ARKA26_GFX_MAX_VERTICES
#define ARKA26_GFX_MAX_VERTICES 8192
#endif // ARKA26_GFX_MAX_VERTICES

typedef enum {
    GFX_BACKEND_NONE,
    GFX_BACKEND_GL33,
    GFX_BACKEND_GLES3,
    GFX_BACKEND_D3D11,
    GFX_BACKEND_ENUM_COUNT
} GfxBackend;

typedef enum {
    GFX_FULLSCREEN = 1 << 0,
    GFX_VSYNC = 1 << 1
} GfxFlags;

typedef struct GfxEngine GfxEngine;
struct GfxEngine {
    GfxBackend backend;
    bool (*init)(GfxEngine *engine, const char *title, u32 width, u32 height, GfxFlags flags);
    void (*setFlags)(GfxEngine *engine, GfxFlags flags);
    void (*setTitle)(GfxEngine *engine, const char *title);
    void (*setSize)(GfxEngine *engine, u32 width, u32 height);
    void (*swap)(GfxEngine *engine);
    void *internal;
};

bool Gfx_Setup(GfxEngine *engine, GfxBackend preferredBackend);

bool Gfx_Init(GfxEngine *engine, const char *title, u32 width, u32 height, GfxFlags flags);

void Gfx_Swap(GfxEngine *engine);

#endif // ARKA26_GFX_H