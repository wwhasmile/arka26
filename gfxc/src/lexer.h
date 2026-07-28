#ifndef LEXER_H
#define LEXER_H

#include <defines.h>

typedef struct {
	const char* start;
	u32 startLine;
	u32 startColumn;

	const char* cur;
	u32 curLine;
	u32 curColumn;
} Lexer;

typedef enum {
	LEXER_TOKEN_NONE,

	LEXER_TOKEN_ERROR,

	LEXER_TOKEN_IDENTIFIER,
	LEXER_TOKEN_STRING_LITERAL,
	LEXER_TOKEN_INT_LITERAL,
	LEXER_TOKEN_FLOAT_LITERAL,
	LEXER_TOKEN_HEX_LITERAL,

	LEXER_TOKEN_DOT,
	LEXER_TOKEN_COMMA,
	LEXER_TOKEN_COLON,
	LEXER_TOKEN_SEMICOLON,

	LEXER_TOKEN_DOLLAR,
	LEXER_TOKEN_AT,

	LEXER_TOKEN_EOF,

	LEXER_TOKEN_ENUM_COUNT
} LexerTokenType;

typedef struct {
	LexerTokenType type;
	u32 line;
	u32 column;
	u32 length;
	const char *lexeme;
} LexerToken;

void Lexer_Initialize(Lexer *lexer, const char *src);

LexerToken Lexer_Next(Lexer *lexer);

#endif // LEXER_H
