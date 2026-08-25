#include "gfx.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <stdlib.h>

typedef struct {
    SDL_Window *window;
    SDL_GLContext glc;
} GfxGL33State;

bool GL33_Gfx_Setup(GfxEngine *engine)
{
    engine->backend = GFX_BACKEND_GL33;
    engine->init = GL33_Gfx_Init;
    engine->swap = GL33_Gfx_Swap;
    return true;
}

bool GL33_Gfx_Init(GfxEngine *engine, const char *title, u32 width, u32 height, GfxFlags flags)
{
    GfxGL33State result = { 0 };
    SDL_WindowFlags windowFlags = SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL;
    if (flags & GFX_FULLSCREEN)
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    result.window = SDL_CreateWindow(title, width, height, windowFlags);
    if (result.window == NULL)
        return false;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    result.glc = SDL_GL_CreateContext(result.window);
    if (result.glc == NULL)
        return false;
    SDL_GL_MakeCurrent(result.window, result.glc);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        return false;
    SDL_ShowWindow(result.window);

    GfxGL33State *state = malloc(sizeof(GfxGL33State));
    *state = result;
    engine->internal = state;

    SDL_GL_SetSwapInterval((flags & GFX_VSYNC) != 0);

    return true;
}

void GL33_Gfx_Swap(GfxEngine *engine)
{
    GfxGL33State *state = engine->internal;
    SDL_GL_SwapWindow(state->window);
}