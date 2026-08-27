#ifndef IMPL_GL33_GFX_H
#define IMPL_GL33_GFX_H

#include <gfx/gfx_driver.h>

bool GL33_GfxDriver_Setup(GfxDriver *engine);

bool GL33_GfxDriver_Init(GfxDriver *engine, const char *title, u32 width, u32 height, GfxFlags flags);

void GL33_GfxDriver_Swap(GfxDriver *engine);

#endif // IMPL_GL33_GFX_H
