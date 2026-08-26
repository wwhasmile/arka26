#include "SDL3/SDL_events.h"
#include "defines.h"
#include <core/platform.h>
#include <SDL3/SDL.h>
#include <stdlib.h>

#define PLATFORM_MAX_EVENTS 256

typedef struct {
    u32 eventFront;
    u32 eventBack;
    PlatformEvent events[PLATFORM_MAX_EVENTS];
} PlatformSDLState;

bool Platform_Init(PlatformState *state)
{
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        return false;

    PlatformSDLState result = {
        0,
        0,
        { 0 }
    };
    PlatformSDLState *internal = malloc(sizeof(PlatformSDLState));
    *internal = result;
    state->internal = internal;
    return true;
}

bool Platform_PollEvent(PlatformState *state, PlatformEvent *event)
{
    PlatformSDLState *internal = state->internal;
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        PlatformEvent event = { 0 };
        event.shared.time = SDL_GetTicks();
        switch (ev.type) {
        case SDL_EVENT_KEY_DOWN:
            if (ev.key.key == SDLK_RETURN && ev.key.mod == SDL_KMOD_LALT) {
                SDL_Window *window = SDL_GetWindowFromEvent(&ev);
                SDL_WindowFlags flags = SDL_GetWindowFlags(window);
                SDL_SetWindowFullscreen(window, (flags & SDL_WINDOW_FULLSCREEN) == 0);
                continue;
            }
            event.key.type = PLATFORM_EVENT_KEY_DOWN;
            event.key.key = ev.key.key;
            event.key.mod = ev.key.mod;
            break;
        case SDL_EVENT_KEY_UP:
            event.key.type = PLATFORM_EVENT_KEY_DOWN;
            event.key.key = ev.key.key;
            event.key.mod = ev.key.mod;
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            event.resize.type = PLATFORM_EVENT_RESIZE;
            break;
        case SDL_EVENT_QUIT:
            event.shared.type = PLATFORM_EVENT_CLOSE;
            break;
        default: continue;
        }

        internal->events[internal->eventFront] = event;
        internal->eventFront = (internal->eventFront + 1) & (ARRAY_LENGTH(internal->events) - 1);
    }
    if (internal->eventFront == internal->eventBack)
        return false;

    *event = internal->events[internal->eventBack];
    internal->eventBack = (internal->eventBack + 1) & (ARRAY_LENGTH(internal->events) - 1);
    return true;
}