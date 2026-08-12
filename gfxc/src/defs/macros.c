#include <gfxc.h>

#define GFXC_MACRO_ONE_ARG(id, arg0, exec) { (id), sizeof((id)) - 1, 1, { arg0 }, exec }

static GfxcValue Deg2Rad(const GfxcAstNode *ast, GfxcAstAnnotation *annotations, u32 arg);

const GfxcMacro GFXC_MACROS[] = {
	GFXC_MACRO_ONE_ARG("Deg2Rad", GFXC_TYPE_FLOAT_LITERAL, Deg2Rad),

	// Tail
	{ 0 }
};

static inline f32 ReadFloat(const GfxcAstNode *ast, GfxcAstAnnotation *annotations, u32 *arg);

GfxcValue Deg2Rad(const GfxcAstNode *ast, GfxcAstAnnotation *annotations, u32 arg)
{
	f32 val = ReadFloat(ast, annotations, &arg) * 3.14159265358979323846f / 180.0f;
	GfxcValue result = { 0 };
	result.floatv.type = GFXC_TYPE_FLOAT;
	result.floatv.value = val;
	return result;
}

inline f32 ReadFloat(const GfxcAstNode *ast, GfxcAstAnnotation *annotations, u32 *arg)
{
	f32 val;
	switch (annotations[*arg].value.data.shared.type) {
	case GFXC_TYPE_INT:
		val = (f32)annotations[*arg].value.data.intv.value;
		break;
	default:
		val = annotations[*arg].value.data.floatv.value;
		break;
	}
	*arg = ast[*arg].next;
	return val;
}

#undef GFXC_MACRO_ONE_ARG
