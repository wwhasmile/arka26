#include <gfxc.h>

#include <core/stack.h>

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
		printf("GFXC version 1\nMade by Hasmile\n\n");
		printf("Usage:\ngfxc input_file output_file");
		return 0;
	}

	char *src;
	{
		FILE *input = fopen(argv[1], "rb");
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

	return 0;
}
