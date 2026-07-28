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
	const char* src = ""
		" ...\n"
		"\ta\t\n;:\n"
		"";
	Lexer_Initialize(&lexer, src);

	LexerToken token = { 0 };
	do {
		token = Lexer_Next(&lexer);
		if (token.type == LEXER_TOKEN_ERROR)
			printf("Error at %u:%u: %s\n", token.line, token.column, token.lexeme);
		else
			printf("token = {\n\t.type = %u,\n\t.line = %u,\n\t.column = %u,\n\t.lexeme = %.*s,\n};\n",
				token.type, token.line, token.column, token.length, token.lexeme);
	} while (token.type != LEXER_TOKEN_EOF);

	return 0;
}
