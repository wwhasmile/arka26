#include <gfxc.h>

const GfxcAttribute GFXC_TEXTURE_ATTRIBUTES[] = {
	{ "path", sizeof("path") - 1, true, { .string.type = GFXC_TYPE_STRING } },
	{ "hasData", sizeof("hasData") - 1, false, { .boolv.type = GFXC_TYPE_BOOL, .boolv.value = true } },
	{ "format", sizeof("format") - 1, false, { .intv.type = GFXC_TYPE_INT, .intv.value = 0 } },
	{ "width", sizeof("width") - 1, false, { .intv.type = GFXC_TYPE_INT, .intv.value = 0 } },
	{ "height", sizeof("height") - 1, false, { .intv.type = GFXC_TYPE_INT, .intv.value = 0 } },
};

const GfxcAttribute GFXC_REGION_ATTRIBUTES[] = {
	{ "texture", sizeof("texture") - 1, true, { .texture.type = GFXC_TYPE_TEXTURE } },
	{ "x", sizeof("x") - 1, false, { .floatv.type = GFXC_TYPE_FLOAT, .floatv.value = 0.0f } },
	{ "y", sizeof("y") - 1, false, { .floatv.type = GFXC_TYPE_FLOAT, .floatv.value = 0.0f } },
	{ "width", sizeof("width") - 1, true, { .floatv.type = GFXC_TYPE_FLOAT } },
	{ "height", sizeof("height") - 1, true, { .floatv.type = GFXC_TYPE_FLOAT } },
};
