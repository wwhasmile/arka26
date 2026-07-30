#include <gfxc.h>

#include <stdio.h>
#include <stdlib.h>

void GFXC_Error(const char *message, u32 line, u32 column, const char *src)
{
	u32 currentLine = 0;
	u32 lineLength = 0;
	const char *actualLine;
	while (*src != '\0' && currentLine < line) {
		++lineLength;
		if (*(src++) == '\n' && ++currentLine < line) {
			actualLine = src;
			lineLength = 0;
		}
	}
	fprintf(stderr, "ERROR: %s at %u:%u\n  %u %.*s\n  %u %*s^\n", message, line, column,
		line, lineLength - 1, actualLine,
		line, column - 1, "");
}

void ShowTree(GfxcAstNode *ast, u32 idx)
{
	while (true) {
		if (ast[idx].type == GFXC_AST_NODE_HEAD) {
			idx = 1;
			continue;
		}
		if (ast[idx].type == GFXC_AST_NODE_TEXTURE) {
			printf("Beginning texture %.*s with these params:\n", ast[idx].data.texture.idLength, ast[idx].data.texture.id);
			ShowTree(ast, idx + 1);
			printf("End of texture definition\n");
		} else if (ast[idx].type == GFXC_AST_NODE_REGION) {
			printf("Beginning region %.*s with these params:\n", ast[idx].data.region.idLength, ast[idx].data.region.id);
			ShowTree(ast, idx + 1);
			printf("End of region definition\n");
		} else if (ast[idx].type == GFXC_AST_NODE_SCRIPT) {
			printf("Beginning script %.*s:\n", ast[idx].data.script.idLength, ast[idx].data.script.id);
			ShowTree(ast, idx + 1);
			printf("End of script definition\n");
		} else if (ast[idx].type == GFXC_AST_NODE_LABEL) {
			printf("Label %.*s\n", ast[idx].data.label.idLength, ast[idx].data.label.id);
		} else if (ast[idx].type == GFXC_AST_NODE_TIME_LABEL) {
			if (ast[idx].data.timeLabel.relative)
				printf("Time label: +%f\n", ast[idx].data.timeLabel.offset);
			else
				printf("Time label: %f\n", ast[idx].data.timeLabel.offset);
		} else if (ast[idx].type == GFXC_AST_NODE_INSTRUCTION) {
			printf("Instruction %.*s, operands: ", ast[idx].data.identifier.idLength, ast[idx].data.identifier.id);
			ShowTree(ast, idx + 1);
			printf("\n");
		} else if (ast[idx].type == GFXC_AST_NODE_IDENTIFIER) {
			printf("%.*s", ast[idx].data.identifier.idLength, ast[idx].data.identifier.id);
			if (ast[idx].next != 0)
				printf(", ");
		} else if (ast[idx].type == GFXC_AST_NODE_FIELD) {
			printf("%.*s : ", ast[idx].data.field.idLength, ast[idx].data.field.id);
			ShowTree(ast, idx + 1);
			printf("\n");
		} else if (ast[idx].type == GFXC_AST_NODE_HEX_LITERAL) {
			printf("0x%X", ast[idx].data.hexLiteral.value);
			if (ast[idx].next != 0)
				printf(", ");
		} else if (ast[idx].type == GFXC_AST_NODE_INT_LITERAL) {
			printf("%d", ast[idx].data.intLiteral.value);
			if (ast[idx].next != 0)
				printf(", ");
		} else if (ast[idx].type == GFXC_AST_NODE_FLOAT_LITERAL) {
			printf("%f", ast[idx].data.floatLiteral.value);
			if (ast[idx].next != 0)
				printf(", ");
		} else if (ast[idx].type == GFXC_AST_NODE_STRING_LITERAL) {
			printf("\"%.*s\"", ast[idx].data.stringLiteral.dataLength, ast[idx].data.stringLiteral.data);
			if (ast[idx].next != 0)
				printf(", ");
		}
		if (ast[idx].next == 0)
			return;
		idx = ast[idx].next;
	}
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
		src[size] = '\n';
		fclose(input);
	}

	GfxcAstNode *ast = GFXC_Parse(src);
	if (ast == NULL) {
		return 1;
	}
	ShowTree(ast, 0);

	return 0;
}

const GfxcRegister GFXC_REGISTERS[] = {
	{ "R0", 0x0000, GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER },
	{ "R1", 0x0001, GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER },
	{ "R2", 0x0002, GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER },
	{ "R3", 0x0003, GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER },
	{ "R4", 0x0004, GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER },
	{ "R5", 0x0005, GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER },
	{ "R6", 0x0006, GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER },
	{ "R7", 0x0007, GFXC_TYPE_RW_REGISTER | GFXC_TYPE_NUMBER },

	{ "IRANDOM", -0x0001, GFXC_TYPE_R_REGISTER | GFXC_TYPE_INT },
	{ "IRANDOM_MIN", -0x0002, GFXC_TYPE_RW_REGISTER | GFXC_TYPE_INT },
	{ "IRANDOM_MAX", -0x0003, GFXC_TYPE_RW_REGISTER | GFXC_TYPE_INT },

	{ "FRANDOM", -0x0004, GFXC_TYPE_R_REGISTER | GFXC_TYPE_FLOAT },
	{ "FRANDOM_MIN", -0x0005, GFXC_TYPE_RW_REGISTER | GFXC_TYPE_FLOAT },
	{ "FRANDOM_MAX", -0x0006, GFXC_TYPE_RW_REGISTER | GFXC_TYPE_FLOAT },

	{ "CRANDOM", -0x0007, GFXC_TYPE_R_REGISTER | GFXC_TYPE_HEX },

	{ NULL, 0x0000, GFXC_TYPE_NONE },
};

static GfxcType GFXC_IMOV_ARGT[] = { GFXC_TYPE_RW_REGISTER, GFXC_TYPE_INT };
static GfxcType GFXC_FMOV_ARGT[] = { GFXC_TYPE_RW_REGISTER, GFXC_TYPE_FLOAT };
const GfxcInstruction GFXC_INSTRUCTIONS[] = {
	{ "nop", 0x00, 0, NULL },

	{ "imov", 0x01, 2, GFXC_IMOV_ARGT },
	{ "fmov", 0x02, 2, GFXC_FMOV_ARGT },

	{ NULL, 0x00, 0, NULL }
};
