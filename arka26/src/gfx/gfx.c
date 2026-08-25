#include "gfx.h"

#include <impl/gl33/gfx.h>

bool Gfx_Setup(GfxEngine *engine, GfxBackend preferredBackend)
{
    if (!GL33_Gfx_Setup(engine)) {
        return false;
    }

    return true;
}

bool Gfx_Init(GfxEngine *engine, const char *title, u32 width, u32 height, GfxFlags flags)
{
    return engine->init(engine, title, width, height, flags);
}

void Gfx_Swap(GfxEngine *engine)
{
    engine->swap(engine);
}