#if defined(__unix__) && !defined(_EMSCRIPTEN_)

#include <gfx/gfx.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(void)
{
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
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

    SDL_Event ev;
    while (true) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                return 0;
        }

        Gfx_Swap(&engine);
    }
}

#endif