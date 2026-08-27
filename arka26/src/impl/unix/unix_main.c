#if defined(__unix__) && !defined(_EMSCRIPTEN_)

#include <gfx/gfx_driver.h>
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

	GfxDriver driver = { 0 };
	if (!GfxDriver_Setup(&driver, GFX_BACKEND_GL33)) {
		SDL_Log("Lol why");
		return 2;
	}
	if (!GfxDriver_Init(&driver, "Arka26", 1280, 960, GFX_VSYNC)) {
		SDL_Log("Fail");
		return 3;
	}

	PlatformEvent ev;
	while (true) {
		while (Platform_PollEvent(&platform, &ev)) {
		if (ev.shared.type == PLATFORM_EVENT_CLOSE)
			return 0;
		}

		GfxDriver_Swap(&driver);
	}
}

#endif
