#include <lexer.h>

#include <stdio.h>

int main(int argc, char **argv)
{
	printf("GFXC version 0\nMade by Hasmile\n\n");

	if (argc < 3)
	{
		printf("Usage:\ngfxc input_file output_file");
		return 1;
	}
	printf("%s\n", argv[0]);

	Lexer lexer = { 0 };
	Lexer_Initialize(&lexer, "");

	return 0;
}
