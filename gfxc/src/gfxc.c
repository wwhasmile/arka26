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

const GfxcSymbol GFXC_DEFAULT_SYMBOLS[] = {
	// Texture formats
	{
		"TEXTURE_FORMAT_RGBA8",
		sizeof("TEXTURE_FORMAT_RGBA8") - 1,
		{
			.integer = { GFXC_TYPE_INT, 0 }
		}
	},
	{
		"TEXTURE_FORMAT_R8",
		sizeof("TEXTURE_FORMAT_R8") - 1,
		{
			.integer = { GFXC_TYPE_INT, 1 }
		}
	},

	// Texture filtering
	{
		"TEXTURE_FILTER_NEAREST",
		sizeof("TEXTURE_FILTER_NEAREST") - 1,
		{
			.integer = { GFXC_TYPE_INT, 0 }
		}
	},
	{
		"TEXTURE_FILTER_LINEAR",
		sizeof("TEXTURE_FILTER_LINEAR") - 1,
		{
			.integer = { GFXC_TYPE_INT, 1 }
		}
	},

	// Origin
	{
		"ORIGIN_OBJECT",
		sizeof("ORIGIN_OBJECT") - 1,
		{
			.integer = { GFXC_TYPE_INT, 0 }
		}
	},
	{
		"ORIGIN_SURFACE",
		sizeof("ORIGIN_SURFACE") - 1,
		{
			.integer = { GFXC_TYPE_INT, 1 }
		}
	},
	{
		"ORIGIN_PARENT",
		sizeof("ORIGIN_PARENT") - 1,
		{
			.integer = { GFXC_TYPE_INT, 2 }
		}
	},

	// General purpose registers
	{ "R0", sizeof("R0") - 1, { .reg = { GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER, 0 } } },
	{ "R1", sizeof("R1") - 1, { .reg = { GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER, 1 } } },
	{ "R2", sizeof("R2") - 1, { .reg = { GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER, 2 } } },
	{ "R3", sizeof("R3") - 1, { .reg = { GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER, 3 } } },
	{ "R4", sizeof("R4") - 1, { .reg = { GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER, 4 } } },
	{ "R5", sizeof("R5") - 1, { .reg = { GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER, 5 } } },
	{ "R6", sizeof("R6") - 1, { .reg = { GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER, 6 } } },
	{ "R7", sizeof("R7") - 1, { .reg = { GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER, 7 } } },

	// Tail, for bounds checking
	{ 0 }
};

static const GfxcType GFXC_ISET_ARGT[] = { GFXC_TYPE_RW_REGISTER | GFXC_TYPE_INT, GFXC_TYPE_INT };
static const GfxcType GFXC_FSET_ARGT[] = { GFXC_TYPE_RW_REGISTER | GFXC_TYPE_FLOAT, GFXC_TYPE_FLOAT };
static const GfxcType GFXC_JMP_ARGT[] = { GFXC_TYPE_LABEL };
const GfxcInstruction GFXC_INSTRUCTIONS[] = {
	{ "iset", sizeof("iset") - 1, 0x01, 2, GFXC_ISET_ARGT },
	{ "fset", sizeof("fset") - 1, 0x02, 2, GFXC_FSET_ARGT },
	{ "jmp", sizeof("jmp") - 1, 0x03, 1, GFXC_JMP_ARGT },

	// Tail, for bounds checking
	{ 0 }
};
