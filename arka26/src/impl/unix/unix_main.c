#if defined(__unix__) && !defined(_EMSCRIPTEN_)

#include <gfx/gfx.h>
#include <core/platform.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(void)
{
    PlatformState platform = { 0 };
    if (!Platform_Init(&platform)) {
        SDL_Log("Failed to initialize: %s", SDL_GetError());
        return 1;
    }

    GfxEngine engine = { 0 };
    if (!Gfx_Setup(&engine, GFX_BACKEND_GL33)) {
        SDL_Log("Lol why");
        return 2;
    }
    if (!Gfx_Init(&engine, "Arka26", 1280, 960, GFX_VSYNC)) {
        SDL_Log("Fail");
        return 3;
    }

    PlatformEvent ev;
    while (true) {
        while (Platform_PollEvent(&platform, &ev)) {
            if (ev.shared.type == PLATFORM_EVENT_CLOSE)
                return 0;
        }

        Gfx_Swap(&engine);
    }
}

#endif