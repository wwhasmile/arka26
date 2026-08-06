#include <gfxc.h>

#include <stack.h>

#include <stdio.h>
#include <stdlib.h>

void GFXC_Error(const char *message, u32 line, u32 column, const char *src)
{
	u32 currentLine = 1;
	while (*src != '\0' && currentLine < line) {
		if (*(src++) == '\n')
			++currentLine;
	}

	u32 length;
	for (length = 0; src[length] != '\n' && src[length] != '\0'; ++length);

	fprintf(stderr, "GFXC: ERROR: %s at %u:%u\n  %u %.*s\n  %u %*s^\n", message, line, column,
		line, length, src,
		line, column - 1, "");
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		printf("GFXC version 0\nMade by Hasmile\n\n");
		printf("Usage:\ngfxc input_file output_file");
		return 0;
	}

	char *src;
	{
		FILE *input = fopen(argv[1], "r");
		if (input == NULL) {
			fprintf(stderr, "GFXC: Input file \"%s\" wasn't found\n", argv[1]);
			return 1;
		}
		fseek(input, 0, SEEK_END);
		u32 size = ftell(input);
		fseek(input, 0, SEEK_SET);
		src = malloc(size + 1);
		fread(src, size, 1, input);
		src[size] = '\0';
		fclose(input);
	}

	GfxcAstNode *ast = GFXC_Parse(src);
	if (ast == NULL) {
		fprintf(stderr, "GFXC: Parsing failed. Exiting...\n");
		return 2;
	}

	GfxcAstAnnotation *annotations = GFXC_Analyze(ast);
	if (annotations == NULL) {
		fprintf(stderr, "GFXC: Semantic analysis failed. Exiting...\n");
		return 3;
	}

	u8 *bytecode = GFXC_Generate(ast, annotations);

	FILE *output = fopen(argv[2], "wb");
	if (output == NULL) {
		fprintf(stderr, "GFXC: Failed to open file %s for writing. Exiting...\n", argv[2]);
		return 4;
	}
	fwrite(bytecode, 1, Stack_Count(bytecode), output);
	fclose(output);

	printf("GFXC: %s has been successfully compiled into %s.\n", argv[1], argv[2]);

	// We're not freeing memory in release builds because... why, system does it, it's end of the program.
	// It's only here in debug for memory checkers not to go rogue.
	#ifndef GFXC_RELEASE
	Stack_Release(bytecode);
	free(annotations);
	Stack_Release(ast);
	free(src);
	#endif // GFXC_RELEASE

	return 0;
}

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
	{ "width", sizeof("width") - 1, false, { .floatv.type = GFXC_TYPE_FLOAT, .floatv.value = 0.0f } },
	{ "height", sizeof("height") - 1, false, { .floatv.type = GFXC_TYPE_FLOAT, .floatv.value = 0.0f } },
};

const GfxcSymbol GFXC_DEFAULT_SYMBOLS[] = {
	// Math constants
	{
		"M_PI",
		sizeof("M_PI") - 1,
		{
			.floatv = { GFXC_TYPE_FLOAT, 3.14159265358979323846f }
		}
	},
	{
		"M_DEG2RAD",
		sizeof("M_DEG2RAD") - 1,
		{
			.floatv = { GFXC_TYPE_FLOAT, 3.14159265358979323846f / 180.0f }
		}
	},
	{
		"M_RAD2DEG",
		sizeof("M_RAD2DEG") - 1,
		{
			.floatv = { GFXC_TYPE_FLOAT, 180.0f / 3.14159265358979323846f }
		}
	},

	// Texture formats
	{
		"TEXTURE_FORMAT_RGBA8",
		sizeof("TEXTURE_FORMAT_RGBA8") - 1,
		{
			.intv = { GFXC_TYPE_INT, 0 }
		}
	},
	{
		"TEXTURE_FORMAT_R8",
		sizeof("TEXTURE_FORMAT_R8") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 }
		}
	},
	{
		"TEXTURE_FORMAT_DEPTH24_STENCIL8",
		sizeof("TEXTURE_FORMAT_DEPTH24_STENCIL8") - 1,
		{
			.intv = { GFXC_TYPE_INT, 2 }
		}
	},

	// Texture filtering
	{
		"TEXTURE_FILTER_NEAREST",
		sizeof("TEXTURE_FILTER_NEAREST") - 1,
		{
			.intv = { GFXC_TYPE_INT, 0 }
		}
	},
	{
		"TEXTURE_FILTER_LINEAR",
		sizeof("TEXTURE_FILTER_LINEAR") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 }
		}
	},

	// Texture repeating
	{
		"TEXTURE_WRAP_CLAMP",
		sizeof("TEXTURE_WRAP_CLAMP") - 1,
		{
			.intv = { GFXC_TYPE_INT, 0 }
		}
	},
	{
		"TEXTURE_WRAP_REPEAT",
		sizeof("TEXTURE_WRAP_REPEAT") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 }
		}
	},
	{
		"TEXTURE_WRAP_MIRROR",
		sizeof("TEXTURE_WRAP_MIRROR") - 1,
		{
			.intv = { GFXC_TYPE_INT, 2 }
		}
	},

	// Origin
	{
		"ORIGIN_OBJECT",
		sizeof("ORIGIN_OBJECT") - 1,
		{
			.intv = { GFXC_TYPE_INT, 0 }
		}
	},
	{
		"ORIGIN_SURFACE",
		sizeof("ORIGIN_SURFACE") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 }
		}
	},
	{
		"ORIGIN_PARENT",
		sizeof("ORIGIN_PARENT") - 1,
		{
			.intv = { GFXC_TYPE_INT, 2 }
		}
	},

	// Anchor
	{
		"ANCHOR_MODE_TOP_LEFT",
		sizeof("ANCHOR_MODE_TOP_LEFT") - 1,
		{
			.intv = { GFXC_TYPE_INT, 0 | 0 }
		}
	},
	{
		"ANCHOR_MODE_TOP_MIDDLE",
		sizeof("ANCHOR_MODE_TOP_MIDDLE") - 1,
		{
			.intv = { GFXC_TYPE_INT, 0 | 1 }
		}
	},
	{
		"ANCHOR_MODE_TOP_RIGHT",
		sizeof("ANCHOR_MODE_TOP_RIGHT") - 1,
		{
			.intv = { GFXC_TYPE_INT, 0 | 2 }
		}
	},
	{
		"ANCHOR_MODE_MIDDLE_LEFT",
		sizeof("ANCHOR_MODE_MIDDLE_LEFT") - 1,
		{
			.intv = { GFXC_TYPE_INT, 4 | 0 }
		}
	},
	{
		"ANCHOR_MODE_CENTER",
		sizeof("ANCHOR_MODE_CENTER") - 1,
		{
			.intv = { GFXC_TYPE_INT, 4 | 1 }
		}
	},
	{
		"ANCHOR_MODE_MIDDLE_RIGHT",
		sizeof("ANCHOR_MODE_MIDDLE_RIGHT") - 1,
		{
			.intv = { GFXC_TYPE_INT, 4 | 2 }
		}
	},
	{
		"ANCHOR_MODE_BOTTOM_LEFT",
		sizeof("ANCHOR_MODE_BOTTOM_LEFT") - 1,
		{
			.intv = { GFXC_TYPE_INT, 8 | 0 }
		}
	},
	{
		"ANCHOR_MODE_BOTTOM_MIDDLE",
		sizeof("ANCHOR_MODE_BOTTOM_MIDDLE") - 1,
		{
			.intv = { GFXC_TYPE_INT, 8 | 1 }
		}
	},
	{
		"ANCHOR_MODE_BOTTOM_RIGHT",
		sizeof("ANCHOR_MODE_BOTTOM_RIGHT") - 1,
		{
			.intv = { GFXC_TYPE_INT, 8 | 2 }
		}
	},

	// Flags
	{
		"FLAG_VISIBLE",
		sizeof("FLAG_VISIBLE") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 << 0 }
		}
	},
	{
		"FLAG_FLIP_X",
		sizeof("FLAG_FLIP_X") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 << 1 }
		}
	},
	{
		"FLAG_FLIP_Y",
		sizeof("FLAG_FLIP_Y") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 << 2 }
		}
	},
	{
		"FLAG_AUTOTURN",
		sizeof("FLAG_AUTOTURN") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 << 3 }
		}
	},
	{
		"FLAG_BLENDING",
		sizeof("FLAG_BLENDING") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 << 4 }
		}
	},
	{
		"FLAG_DEPTH_TESTING",
		sizeof("FLAG_DEPTH_TESTING") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 << 5 }
		}
	},
	{
		"FLAG_WRITE_DEPTH",
		sizeof("FLAG_WRITE_DEPTH") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 << 6 }
		}
	},
	{
		"FLAG_CULLING",
		sizeof("FLAG_CULLING") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 << 7 }
		}
	},
	{
		"FLAG_INTERPOLATE",
		sizeof("FLAG_INTERPOLATE") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 << 8 }
		}
	},

	// Render
	{
		"RENDER_PIXEL",
		sizeof("RENDER_PIXEL") - 1,
		{
			.intv = { GFXC_TYPE_INT, 0 }
		}
	},
	{
		"RENDER_2D",
		sizeof("RENDER_2D") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 }
		}
	},
	{
		"RENDER_BILLBOARD",
		sizeof("RENDER_BILLBOARD") - 1,
		{
			.intv = { GFXC_TYPE_INT, 2 }
		}
	},
	{
		"RENDER_3D",
		sizeof("RENDER_3D") - 1,
		{
			.intv = { GFXC_TYPE_INT, 3 }
		}
	},

	// Blend
	{
		"BLEND_ZERO",
		sizeof("BLEND_ZERO") - 1,
		{
			.intv = { GFXC_TYPE_INT, 0 }
		}
	},
	{
		"BLEND_ONE",
		sizeof("BLEND_ONE") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 }
		}
	},
	{
		"BLEND_SRC_COLOR",
		sizeof("BLEND_SRC_COLOR") - 1,
		{
			.intv = { GFXC_TYPE_INT, 2 }
		}
	},
	{
		"BLEND_INV_SRC_COLOR",
		sizeof("BLEND_INV_SRC_COLOR") - 1,
		{
			.intv = { GFXC_TYPE_INT, 3 }
		}
	},
	{
		"BLEND_SRC_ALPHA",
		sizeof("BLEND_SRC_ALPHA") - 1,
		{
			.intv = { GFXC_TYPE_INT, 4 }
		}
	},
	{
		"BLEND_INV_SRC_ALPHA",
		sizeof("BLEND_INV_SRC_ALPHA") - 1,
		{
			.intv = { GFXC_TYPE_INT, 5 }
		}
	},
	{
		"BLEND_DEST_ALPHA",
		sizeof("BLEND_DEST_ALPHA") - 1,
		{
			.intv = { GFXC_TYPE_INT, 6 }
		}
	},
	{
		"BLEND_INV_DEST_ALPHA",
		sizeof("BLEND_INV_DEST_ALPHA") - 1,
		{
			.intv = { GFXC_TYPE_INT, 7 }
		}
	},
	{
		"BLEND_DEST_COLOR",
		sizeof("BLEND_DEST_COLOR") - 1,
		{
			.intv = { GFXC_TYPE_INT, 8 }
		}
	},
	{
		"BLEND_INV_DEST_COLOR",
		sizeof("BLEND_INV_DEST_COLOR") - 1,
		{
			.intv = { GFXC_TYPE_INT, 9 }
		}
	},

	// Blend op
	{
		"BLEND_OP_ADD",
		sizeof("BLEND_OP_ADD") - 1,
		{
			.intv = { GFXC_TYPE_INT, 0 }
		}
	},
	{
		"BLEND_OP_SUB",
		sizeof("BLEND_OP_SUB") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 }
		}
	},
	{
		"BLEND_OP_REVERSE_SUB",
		sizeof("BLEND_OP_REVERSE_SUB") - 1,
		{
			.intv = { GFXC_TYPE_INT, 2 }
		}
	},
	{
		"BLEND_OP_REVERSE_MIN",
		sizeof("BLEND_OP_REVERSE_MIN") - 1,
		{
			.intv = { GFXC_TYPE_INT, 3 }
		}
	},
	{
		"BLEND_OP_REVERSE_MAX",
		sizeof("BLEND_OP_REVERSE_MAX") - 1,
		{
			.intv = { GFXC_TYPE_INT, 4 }
		}
	},

	// Comparison
	{
		"COMPARISON_NEVER",
		sizeof("COMPARISON_NEVER") - 1,
		{
			.intv = { GFXC_TYPE_INT, 0 }
		}
	},
	{
		"COMPARISON_LESS",
		sizeof("COMPARISON_LESS") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 }
		}
	},
	{
		"COMPARISON_EQUAL",
		sizeof("COMPARISON_EQUAL") - 1,
		{
			.intv = { GFXC_TYPE_INT, 2 }
		}
	},
	{
		"COMPARISON_LEQUAL",
		sizeof("COMPARISON_LEQUAL") - 1,
		{
			.intv = { GFXC_TYPE_INT, 3 }
		}
	},
	{
		"COMPARISON_GREATER",
		sizeof("COMPARISON_GREATER") - 1,
		{
			.intv = { GFXC_TYPE_INT, 4 }
		}
	},
	{
		"COMPARISON_NEQUAL",
		sizeof("COMPARISON_NEQUAL") - 1,
		{
			.intv = { GFXC_TYPE_INT, 5 }
		}
	},
	{
		"COMPARISON_GEQUAL",
		sizeof("COMPARISON_GEQUAL") - 1,
		{
			.intv = { GFXC_TYPE_INT, 6 }
		}
	},
	{
		"COMPARISON_ALWAYS",
		sizeof("COMPARISON_ALWAYS") - 1,
		{
			.intv = { GFXC_TYPE_INT, 7 }
		}
	},

	// Cull
	{
		"CUL_BACK",
		sizeof("CULL_BACK") - 1,
		{
			.intv = { GFXC_TYPE_INT, 0 }
		}
	},
	{
		"CULL_FRONT",
		sizeof("CULL_FRONT") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 }
		}
	},
	{
		"CULL_FRONT_BACK",
		sizeof("CULL_FRONT_BACK") - 1,
		{
			.intv = { GFXC_TYPE_INT, 2 }
		}
	},

	// Interpolation
	{
		"INTERPOLATION_LINEAR",
		sizeof("INTERPOLATION_LINEAR") - 1,
		{
			.intv = { GFXC_TYPE_INT, 0 }
		}
	},
	{
		"INTERPOLATION_EASE_IN_QUAD",
		sizeof("INTERPOLATION_EASE_IN_QUAD") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 }
		}
	},
	{
		"INTERPOLATION_EASE_OUT_QUAD",
		sizeof("INTERPOLATION_EASE_OUT_QUAD") - 1,
		{
			.intv = { GFXC_TYPE_INT, 2 }
		}
	},
	{
		"INTERPOLATION_EASE_IN_OUT_QUAD",
		sizeof("INTERPOLATION_EASE_IN_OUT_QUAD") - 1,
		{
			.intv = { GFXC_TYPE_INT, 3 }
		}
	},
	{
		"INTERPOLATION_EASE_IN_QUAD",
		sizeof("INTERPOLATION_EASE_IN_CUBIC") - 1,
		{
			.intv = { GFXC_TYPE_INT, 4 }
		}
	},
	{
		"INTERPOLATION_EASE_OUT_QUAD",
		sizeof("INTERPOLATION_EASE_OUT_CUBIC") - 1,
		{
			.intv = { GFXC_TYPE_INT, 5 }
		}
	},
	{
		"INTERPOLATION_EASE_IN_OUT_QUAD",
		sizeof("INTERPOLATION_EASE_IN_OUT_CUBIC") - 1,
		{
			.intv = { GFXC_TYPE_INT, 6 }
		}
	},
	{
		"INTERPOLATION_EASE_IN_EXP",
		sizeof("INTERPOLATION_EASE_IN_EXP") - 1,
		{
			.intv = { GFXC_TYPE_INT, 7 }
		}
	},
	{
		"INTERPOLATION_EASE_OUT_EXP",
		sizeof("INTERPOLATION_EASE_OUT_EXP") - 1,
		{
			.intv = { GFXC_TYPE_INT, 8 }
		}
	},
	{
		"INTERPOLATION_EASE_IN_OUT_EXP",
		sizeof("INTERPOLATION_EASE_IN_OUT_EXP") - 1,
		{
			.intv = { GFXC_TYPE_INT, 9 }
		}
	},
	{
		"INTERPOLATION_EASE_IN_BACK",
		sizeof("INTERPOLATION_EASE_IN_BACK") - 1,
		{
			.intv = { GFXC_TYPE_INT, 10 }
		}
	},
	{
		"INTERPOLATION_EASE_OUT_BACK",
		sizeof("INTERPOLATION_EASE_OUT_BACK") - 1,
		{
			.intv = { GFXC_TYPE_INT, 11 }
		}
	},
	{
		"INTERPOLATION_EASE_IN_OUT_BACK",
		sizeof("INTERPOLATION_EASE_IN_OUT_BACK") - 1,
		{
			.intv = { GFXC_TYPE_INT, 12 }
		}
	},
	{
		"INTERPOLATION_EASE_IN_ELASTIC",
		sizeof("INTERPOLATION_EASE_IN_ELASTIC") - 1,
		{
			.intv = { GFXC_TYPE_INT, 13 }
		}
	},
	{
		"INTERPOLATION_EASE_OUT_ELASTIC",
		sizeof("INTERPOLATION_EASE_OUT_ELASTIC") - 1,
		{
			.intv = { GFXC_TYPE_INT, 14 }
		}
	},
	{
		"INTERPOLATION_EASE_IN_OUT_ELASTIC",
		sizeof("INTERPOLATION_EASE_IN_OUT_ELASTIC") - 1,
		{
			.intv = { GFXC_TYPE_INT, 15 }
		}
	},
	{
		"INTERPOLATION_EASE_IN_BOUNCE",
		sizeof("INTERPOLATION_EASE_IN_BOUNCE") - 1,
		{
			.intv = { GFXC_TYPE_INT, 16 }
		}
	},
	{
		"INTERPOLATION_EASE_OUT_BOUNCE",
		sizeof("INTERPOLATION_EASE_OUT_BOUNCE") - 1,
		{
			.intv = { GFXC_TYPE_INT, 17 }
		}
	},
	{
		"INTERPOLATION_EASE_IN_OUT_BOUNCE",
		sizeof("INTERPOLATION_EASE_IN_OUT_BOUNCE") - 1,
		{
			.intv = { GFXC_TYPE_INT, 18 }
		}
	},

	// Gradient
	{
		"GRADIENT_A",
		sizeof("GRADIENT_A") - 1,
		{
			.intv = { GFXC_TYPE_INT, 0 }
		}
	},
	{
		"GRADIENT_B",
		sizeof("GRADIENT_B") - 1,
		{
			.intv = { GFXC_TYPE_INT, 1 }
		}
	},
	{
		"GRADIENT_AB",
		sizeof("GRADIENT_AB") - 1,
		{
			.intv = { GFXC_TYPE_INT, 2 }
		}
	},
	{
		"GRADIENT_BA",
		sizeof("GRADIENT_BA") - 1,
		{
			.intv = { GFXC_TYPE_INT, 3 }
		}
	},

	// Registers
	{ "x0", sizeof("x0") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 0 } } },
	{ "x1", sizeof("x1") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 1 } } },
	{ "x2", sizeof("x2") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 2 } } },
	{ "x3", sizeof("x3") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 3 } } },
	{ "x4", sizeof("x4") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 4 } } },
	{ "x5", sizeof("x5") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 5 } } },
	{ "x6", sizeof("x6") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 6 } } },
	{ "x7", sizeof("x7") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 7 } } },
	{ "x8", sizeof("x8") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 8 } } },
	{ "x9", sizeof("x9") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 9 } } },
	{ "x10", sizeof("x10") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 10 } } },
	{ "x11", sizeof("x11") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 11 } } },
	{ "x12", sizeof("x12") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 12 } } },
	{ "x13", sizeof("x13") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 13 } } },
	{ "x14", sizeof("x14") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 14 } } },
	{ "x15", sizeof("x15") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 15 } } },
	{ "x16", sizeof("x16") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 16 } } },
	{ "x17", sizeof("x17") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 17 } } },
	{ "x18", sizeof("x18") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 18 } } },
	{ "x19", sizeof("x19") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 19 } } },
	{ "x20", sizeof("x20") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 20 } } },
	{ "x21", sizeof("x21") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 21 } } },
	{ "x22", sizeof("x22") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 22 } } },
	{ "x23", sizeof("x23") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 23 } } },
	{ "x24", sizeof("x24") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 24 } } },
	{ "x25", sizeof("x25") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 25 } } },
	{ "x26", sizeof("x26") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 26 } } },
	{ "x27", sizeof("x27") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 27 } } },
	{ "x28", sizeof("x28") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 28 } } },
	{ "x29", sizeof("x29") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 29 } } },
	{ "x30", sizeof("x30") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 30 } } },
	{ "x31", sizeof("x31") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 31 } } },
	{ "f0", sizeof("f0") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 0 } } },
	{ "f1", sizeof("f1") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 1 } } },
	{ "f2", sizeof("f2") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 2 } } },
	{ "f3", sizeof("f3") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 3 } } },
	{ "f4", sizeof("f4") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 4 } } },
	{ "f5", sizeof("f5") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 5 } } },
	{ "f6", sizeof("f6") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 6 } } },
	{ "f7", sizeof("f7") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 7 } } },
	{ "f8", sizeof("f8") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 8 } } },
	{ "f9", sizeof("f9") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 9 } } },
	{ "f10", sizeof("f10") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 10 } } },
	{ "f11", sizeof("f11") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 11 } } },
	{ "f12", sizeof("f12") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 12 } } },
	{ "f13", sizeof("f13") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 13 } } },
	{ "f14", sizeof("f14") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 14 } } },
	{ "f15", sizeof("f15") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 15 } } },
	{ "f16", sizeof("f16") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 16 } } },
	{ "f17", sizeof("f17") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 17 } } },
	{ "f18", sizeof("f18") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 18 } } },
	{ "f19", sizeof("f19") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 19 } } },
	{ "f20", sizeof("f20") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 20 } } },
	{ "f21", sizeof("f21") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 21 } } },
	{ "f22", sizeof("f22") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 22 } } },
	{ "f23", sizeof("f23") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 23 } } },
	{ "f24", sizeof("f24") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 24 } } },
	{ "f25", sizeof("f25") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 25 } } },
	{ "f26", sizeof("f26") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 26 } } },
	{ "f27", sizeof("f27") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 27 } } },
	{ "f28", sizeof("f28") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 28 } } },
	{ "f29", sizeof("f29") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 29 } } },
	{ "f30", sizeof("f30") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 30 } } },
	{ "f31", sizeof("f31") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 31 } } },

	// Tail, for bounds checking
	{ 0 }
};

const GfxcInstruction GFXC_INSTRUCTIONS[] = {
	{ "nop", sizeof("nop") - 1, 0, { 0 } },

	{ "jump", sizeof("jump") - 1, 2, { GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "wait", sizeof("wait") - 1, 1, { GFXC_TYPE_FLOAT } },
	{ "sleep", sizeof("sleep") - 1, 0, { 0 } },
	{ "trigger", sizeof("trigger") - 1, 2, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_LABEL } },
	{ "untrigger", sizeof("untrigger") - 1, 0, { 0 } },
	{ "freeze", sizeof("freeze") - 1, 0, { 0 } },
	{ "return", sizeof("ret") - 1, 0, { 0 } },

	{ "set", sizeof("set") - 1, 2, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT } },
	{ "add", sizeof("add") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_INT } },
	{ "sub", sizeof("sub") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_INT } },
	{ "xor", sizeof("xor") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_INT } },
	{ "or", sizeof("or") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_INT } },
	{ "and", sizeof("and") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_INT } },
	{ "sl", sizeof("sl") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_INT } },
	{ "sr", sizeof("sr") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_INT } },
	{ "mul", sizeof("mul") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_INT } },
	{ "div", sizeof("div") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_INT } },
	{ "rem", sizeof("rem") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_INT } },

	{ "setr", sizeof("setr") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_INT } },

	{ "beq", sizeof("beq") - 1, 4, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "bne", sizeof("bne") - 1, 4, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "blt", sizeof("blt") - 1, 4, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "ble", sizeof("ble") - 1, 4, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "bgt", sizeof("bgt") - 1, 4, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "bge", sizeof("bge") - 1, 4, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT  } },

	{ "fset", sizeof("fset") - 1, 2, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT } },
	{ "fadd", sizeof("fadd") - 1, 3, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },
	{ "fsub", sizeof("fsub") - 1, 3, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },
	{ "fmul", sizeof("fmul") - 1, 3, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },
	{ "fdiv", sizeof("fdiv") - 1, 3, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },
	{ "fmod", sizeof("fmod") - 1, 3, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },

	{ "fsetr", sizeof("fsetr") - 1, 3, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },

	{ "fcfi", sizeof("fcfi") - 1, 2, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_INT_REGISTER } },
	{ "fc2i", sizeof("fc2i") - 1, 2, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_FLOAT_REGISTER } },

	{ "fsin", sizeof("fsin") - 1, 2, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT } },
	{ "fasin", sizeof("fasin") - 1, 2, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT } },
	{ "fcos", sizeof("fcos") - 1, 2, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT } },
	{ "facos", sizeof("facos") - 1, 2, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT } },
	{ "ftan", sizeof("ftan") - 1, 2, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT } },
	{ "fatan", sizeof("fatan") - 1, 2, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT } },
	{ "fatan2", sizeof("fatan2") - 1, 3, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },

	{ "fbeq", sizeof("fbeq") - 1, 4, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "fbne", sizeof("fbne") - 1, 4, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "fblt", sizeof("fblt") - 1, 4, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "fble", sizeof("fble") - 1, 4, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "fbgt", sizeof("fbgt") - 1, 4, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "fbge", sizeof("fbge") - 1, 4, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },

	{ "region", sizeof("region") - 1, 1, { GFXC_TYPE_REGION } },
	{ "regionr", sizeof("regionr") - 1, 2, { GFXC_TYPE_REGION, GFXC_TYPE_REGION } },
	{ "render", sizeof("render") - 1, 1, { GFXC_TYPE_INT } },
	{ "layer", sizeof("layer") - 1, 1, { GFXC_TYPE_INT } },
	{ "filter", sizeof("filter") - 1, 1, { GFXC_TYPE_INT } },
	{ "wrap", sizeof("wrap") - 1, 2, { GFXC_TYPE_INT, GFXC_TYPE_INT } },
	{ "flags", sizeof("flags") - 1, 2, { GFXC_TYPE_INT, GFXC_TYPE_BOOL } },
	{ "blend.rgb", sizeof("blend.rgb") - 1, 2, { GFXC_TYPE_INT, GFXC_TYPE_INT } },
	{ "blend.a", sizeof("blend.a") - 1, 2, { GFXC_TYPE_INT, GFXC_TYPE_INT } },
	{ "blendop.rgb", sizeof("blendop.rgb") - 1, 1, { GFXC_TYPE_INT } },
	{ "blendop.a", sizeof("blendop.a") - 1, 1, { GFXC_TYPE_INT } },
	{ "depthf", sizeof("depthf") - 1, 1, { GFXC_TYPE_INT } },
	{ "cull", sizeof("cull") - 1, 1, { GFXC_TYPE_INT } },

	{ "anchor", sizeof("anchor") - 1, 1, { GFXC_TYPE_INT } },
	{ "origin", sizeof("origin") - 1, 1, { GFXC_TYPE_INT } },
	{ "offset", sizeof("offset") - 1, 2, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },
	{ "pos", sizeof("pos") - 1, 3, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },
	{ "rot", sizeof("rot") - 1, 3, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },
	{ "scale", sizeof("scale") - 1, 2, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },
	{ "rgb.a", sizeof("rgb.a") - 1, 1, { GFXC_TYPE_HEX } },
	{ "rgb.b", sizeof("rgb.b") - 1, 1, { GFXC_TYPE_HEX } },
	{ "a.a", sizeof("a.a") - 1, 1, { GFXC_TYPE_FLOAT } },
	{ "a.b", sizeof("a.b") - 1, 1, { GFXC_TYPE_FLOAT } },
	{ "lvel", sizeof("lvel") - 1, 3, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },
	{ "avel", sizeof("avel") - 1, 3, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },
	{ "scroll.x", sizeof("scroll.x") - 1, 1, { GFXC_TYPE_FLOAT } },
	{ "scroll.y", sizeof("scroll.y") - 1, 1, { GFXC_TYPE_FLOAT } },
	{ "scale.uv", sizeof("scale.uv") - 1, 1, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },
	{ "tw.offset", sizeof("tw.offset") - 1, 4, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_INT } },
	{ "tw.pos", sizeof("tw.pos") - 1, 5, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_INT } },
	{ "tw.rot", sizeof("tw.rot") - 1, 5, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_INT } },
	{ "tw.scale", sizeof("tw.scale") - 1, 4, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_INT } },
	{ "tw.rgba", sizeof("tw.rgba") - 1, 3, { GFXC_TYPE_HEX, GFXC_TYPE_FLOAT, GFXC_TYPE_INT } },
	{ "tw.rgbb", sizeof("tw.rgbb") - 1, 3, { GFXC_TYPE_HEX, GFXC_TYPE_FLOAT, GFXC_TYPE_INT } },
	{ "tw.aa", sizeof("tw.a.a") - 1, 3, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_INT } },
	{ "tw.ab", sizeof("tw.a.b") - 1, 3, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_INT } },
	{ "tw.lvel", sizeof("tw.lvel") - 1, 5, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_INT } },
	{ "tw.avel", sizeof("tw.avel") - 1, 5, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_INT } },
	{ "tw.avel", sizeof("tw.avel") - 1, 5, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_INT } },
	{ "tw.scrollx", sizeof("tw.scroll.x") - 1, 1, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_INT } },
	{ "tw.scrolly", sizeof("tw.scroll.y") - 1, 1, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_INT } },
	{ "tw.scaleuv", sizeof("tw.scale.uv") - 1, 1, { GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_INT } },
	{ "fade", sizeof("fade") - 1, 3, { GFXC_TYPE_BOOL, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },
	{ "grad", sizeof("grad") - 1, 1, { GFXC_TYPE_INT } },

	{ "new", sizeof("new") - 1, 2, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_SCRIPT } },
	{ "newpos", sizeof("newpos") - 1, 5, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_SCRIPT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },
	{ "detnew", sizeof("detnew") - 1, 1, { GFXC_TYPE_SCRIPT } },
	{ "detnewpos", sizeof("detnewpos") - 1, 4, { GFXC_TYPE_SCRIPT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT } },
	{ "inh", sizeof("inh") - 1, 0, { 0 } },
	{ "kill", sizeof("kill") - 1, 1, { GFXC_TYPE_INT_REGISTER } },

	// Tail, for bounds checking
	{ 0 }
};
