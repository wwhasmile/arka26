#include "stack.h"
#include <gfxc.h>

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

	fprintf(stderr, "ERROR: %s at %u:%u\n  %u %.*s\n  %u %*s^\n", message, line, column,
		line, length, src,
		line, column - 1, "");
}

int main(int argc, char **argv)
{
	printf("GFXC version 0\nMade by Hasmile\n\n");

	if (argc < 3)
	{
		printf("Usage:\ngfxc input_file output_file");
		return 1;
	}
	fflush(stdout);

	char *src;
	{
		FILE *input = fopen(argv[1], "r");
		if (input == NULL) {
			printf("Input file \"%s\" wasn't found\n", argv[1]);
			return 2;
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
		return 1;
	}

	GfxcAstAnnotation *annotations = GFXC_Analyze(ast);

	Stack_Release(ast);
	free(annotations);
	free(src);

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

	// Registers
	{ "x0", sizeof("x0") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 0 } } },
	{ "x1", sizeof("x1") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 1 } } },
	{ "x2", sizeof("x2") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 2 } } },
	{ "x3", sizeof("x3") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 3 } } },
	{ "x4", sizeof("x4") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 4 } } },
	{ "x5", sizeof("x5") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 5 } } },
	{ "x6", sizeof("x6") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 6 } } },
	{ "x7", sizeof("x7") - 1, { .reg = { GFXC_TYPE_INT_REGISTER, 7 } } },
	{ "f0", sizeof("f0") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 0 } } },
	{ "f1", sizeof("f1") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 1 } } },
	{ "f2", sizeof("f2") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 2 } } },
	{ "f3", sizeof("f3") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 3 } } },
	{ "f4", sizeof("f4") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 4 } } },
	{ "f5", sizeof("f5") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 5 } } },
	{ "f6", sizeof("f6") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 6 } } },
	{ "f7", sizeof("f7") - 1, { .reg = { GFXC_TYPE_FLOAT_REGISTER, 7 } } },

	// Tail, for bounds checking
	{ 0 }
};

const GfxcInstruction GFXC_INSTRUCTIONS[] = {
	{ "nop", sizeof("nop") - 1, 0, { 0 } },

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

	{ "setr", sizeof("setr") - 1, 2, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_INT } },

	{ "beq", sizeof("beq") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "bne", sizeof("bne") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "blt", sizeof("blt") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "ble", sizeof("ble") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "bgt", sizeof("bgt") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "bge", sizeof("bge") - 1, 3, { GFXC_TYPE_INT_REGISTER, GFXC_TYPE_INT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },

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

	{ "fbeq", sizeof("fbeq") - 1, 3, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "fbne", sizeof("fbne") - 1, 3, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "fblt", sizeof("fblt") - 1, 3, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "fble", sizeof("fble") - 1, 3, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "fbgt", sizeof("fbgt") - 1, 3, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },
	{ "fbge", sizeof("fbge") - 1, 3, { GFXC_TYPE_FLOAT_REGISTER, GFXC_TYPE_FLOAT, GFXC_TYPE_LABEL, GFXC_TYPE_FLOAT } },

	// Tail, for bounds checking
	{ 0 }
};
