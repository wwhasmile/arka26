#include <lexer.h>

#include <stdio.h>

static const char *lexerTokenTypeToString[] = {
	"LEXER_TOKEN_NONE",

	"LEXER_TOKEN_ERROR",

	"LEXER_TOKEN_IDENTIFIER",
	"LEXER_TOKEN_STRING_LITERAL",
	"LEXER_TOKEN_INT_LITERAL",
	"LEXER_TOKEN_FLOAT_LITERAL",
	"LEXER_TOKEN_HEX_LITERAL",

	"LEXER_TOKEN_DOT",
	"LEXER_TOKEN_COMMA",
	"LEXER_TOKEN_COLON",
	"LEXER_TOKEN_SEMICOLON",

	"LEXER_TOKEN_DOLLAR",
	"LEXER_TOKEN_PERCENT",

	"LEXER_TOKEN_EOF",
};

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
		"0xFFFFFF\n"
		"0xZ$$@%@$\t\n"
		"# HELLOOOOOOOO\n"
		"0 25 25f 1.26. 1.23f .25f 6.f _testIdentifier123 test_ide_2\n"
		"\"HELLOOOOOOOOOOOOOOOO\\nOOO!!!!!!!!!!@@@@@ AAAAAAA____ 25.0f\"\n"
		"\"Oops, EOF"
		"";
	Lexer_Initialize(&lexer, src);

	LexerToken token = { 0 };
	do {
		token = Lexer_Next(&lexer);
		if (token.type == LEXER_TOKEN_ERROR)
			printf("Error at %u:%u: %s\n", token.line, token.column, token.lexeme);
		else
			printf("token = {\n\t.type = %s,\n\t.line = %u,\n\t.column = %u,\n\t.lexeme = %.*s,\n};\n",
				lexerTokenTypeToString[token.type], token.line, token.column, token.length, token.lexeme);
	} while (token.type != LEXER_TOKEN_EOF);

	return 0;
}
