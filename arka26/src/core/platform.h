#ifndef ARKA26_PLATFORM_H
#define ARKA26_PLATFORM_H

#include <defines.h>

typedef enum {
	PLATFORM_EVENT_NONE,
	PLATFORM_EVENT_KEY_DOWN,
	PLATFORM_EVENT_KEY_UP,
	PLATFORM_EVENT_KEY_CHAR,
	PLATFORM_EVENT_RESIZE,
	PLATFORM_EVENT_CLOSE,
	PLATFORM_EVENT_ENUM_COUNT
	} PlatformEventType;

typedef union {
	struct {
		u64 time;
		PlatformEventType type;
	} shared;
	struct {
		u64 time;
		PlatformEventType type;
		u32 key;
		u32 mod;
	} key;
	struct {
		u64 time;
		PlatformEventType type;
		u32 width;
		u32 height;
	} resize;
} PlatformEvent;

typedef struct {
	void *internal;
} PlatformState;

bool Platform_Init(PlatformState *state);

bool Platform_PollEvent(PlatformState *state, PlatformEvent *event);

#endif // ARKA26_PLATFORM_H
