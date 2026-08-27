#include "gfx_driver.h"

#include <impl/gl33/gl33_gfx_driver.h>

bool GfxDriver_Setup(GfxDriver *engine, GfxBackend preferredBackend)
{
    if (!GL33_GfxDriver_Setup(engine)) {
        return false;
    }

    return true;
}

bool GfxDriver_Init(GfxDriver *engine, const char *title, u32 width, u32 height, GfxFlags flags)
{
    return engine->init(engine, title, width, height, flags);
}

void GfxDriver_Swap(GfxDriver *engine)
{
    engine->swap(engine);
}
