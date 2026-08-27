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

typedef struct GfxDriver GfxDriver;
struct GfxDriver {
	GfxBackend backend;
	bool (*init)(GfxDriver *engine, const char *title, u32 width, u32 height, GfxFlags flags);
	void (*setFlags)(GfxDriver *engine, GfxFlags flags);
	void (*setTitle)(GfxDriver *engine, const char *title);
	void (*setSize)(GfxDriver *engine, u32 width, u32 height);
	void (*swap)(GfxDriver *engine);
	void *internal;
};

bool GfxDriver_Setup(GfxDriver *engine, GfxBackend preferredBackend);

bool GfxDriver_Init(GfxDriver *engine, const char *title, u32 width, u32 height, GfxFlags flags);

void GfxDriver_Swap(GfxDriver *engine);

#endif // ARKA26_GFX_H
