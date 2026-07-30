#include <gfxc.h>

#include <stdio.h>

void GFXC_Error(const char *message, u32 line, u32 column)
{
	fprintf(stderr, "ERROR: %s at %u:%u\n", message, line, column);
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
	printf("%s\n", argv[0]);

	const char* src = ""
		"tex surface0\n"
		"\tfile: \"@R\""
		"\thasData: false\n"
		"\twidth: 1280\n"
		"\theight: -1024.2f\n"
		"end\n"
		"tex b\n"
		"\tfuckYou: false\n"
		"end\n"
		"\n"
		"regi surfaceBg\n"
		"\ttex: surface0\n"
		"\tx: 0\n"
		"\ty: 0\n"
		"\twidth: 1280\n"
		"\theight: 1024\n"
		"end\n"
		"\n"
		"script ba\n"
		"\tlabababababa:\n"
		"\t+24:\n"
		"\t24:\n"
		"\tmov 25, a\n"
		"\tmov 2, a,\n"
		"end\n"
		"";

	GfxcAstNode *ast = GFXC_Parse(src);
	if (ast == NULL) {
		return 1;
	}
	ShowTree(ast, 0);

	return 0;
}
