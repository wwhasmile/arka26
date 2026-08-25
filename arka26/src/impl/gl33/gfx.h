#ifndef IMPL_GL33_GFX_H
#define IMPL_GL33_GFX_H

#include <gfx/gfx.h>

bool GL33_Gfx_Setup(GfxEngine *engine);

bool GL33_Gfx_Init(GfxEngine *engine, const char *title, u32 width, u32 height, GfxFlags flags);

void GL33_Gfx_Swap(GfxEngine *engine);

#endif // IMPL_GL33_GFX_H