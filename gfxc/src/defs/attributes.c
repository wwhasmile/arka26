#include <gfxc.h>

#define GFXC_ATTRIBUTE_TEXTURE(x, m, v) { (x), sizeof((x)) - 1, (m), { .texture.type = GFXC_TYPE_TEXTURE, .texture.id = (v) } }
#define GFXC_ATTRIBUTE_INT(x, m, v) { (x), sizeof((x)) - 1, (m), { .intv.type = GFXC_TYPE_INT, .intv.value = (v) } }
#define GFXC_ATTRIBUTE_FLOAT(x, m, v) { (x), sizeof((x)) - 1, (m), { .floatv.type = GFXC_TYPE_FLOAT, .floatv.value = (v) } }
#define GFXC_ATTRIBUTE_BOOL(x, m, v) { (x), sizeof((x)) - 1, (m), { .boolv.type = GFXC_TYPE_BOOL, .boolv.value = (v) } }
#define GFXC_ATTRIBUTE_STRING(x, m, v) \
	{ \
		(x), \
		sizeof((x)) - 1, \
		(m),\
		{ \
			.string.type = GFXC_TYPE_STRING, \
			.string.dataLength = sizeof((v)) - 1, \
			.string.data = (v) \
		} \
	}

const GfxcAttribute GFXC_TEXTURE_ATTRIBUTES[] = {
	GFXC_ATTRIBUTE_STRING("path", true, ""),
	GFXC_ATTRIBUTE_BOOL("hasData", false, true),
	GFXC_ATTRIBUTE_INT("format", false, 0),
	GFXC_ATTRIBUTE_INT("width", false, 0),
	GFXC_ATTRIBUTE_INT("height", false, 0),
};

const GfxcAttribute GFXC_REGION_ATTRIBUTES[] = {
	GFXC_ATTRIBUTE_TEXTURE("texture", true, 0),
	GFXC_ATTRIBUTE_FLOAT("x", false, 0.0f),
	GFXC_ATTRIBUTE_FLOAT("y", false, 0.0f),
	GFXC_ATTRIBUTE_FLOAT("width", false, 0.0f),
	GFXC_ATTRIBUTE_FLOAT("height", false, 0.0f),
};

#undef GFXC_ATTRIBUTE_STRING
#undef GFXC_ATTRIBUTE_BOOL
#undef GFXC_ATTRIBUTE_FLOAT
#undef GFXC_ATTRIBUTE_INT
#undef GFXC_ATTRIBUTE_TEXTURE
