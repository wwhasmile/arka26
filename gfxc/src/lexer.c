#include "lexer.h"

#include <ctype.h>
#include <string.h>

static LexerToken Identifier(Lexer *lexer);

static LexerToken String(Lexer *lexer);

static LexerToken Hexadecimal(Lexer *lexer);

static LexerToken Number(Lexer *lexer, bool isFloat);

static inline LexerToken MakeToken(const Lexer *lexer, LexerTokenType type);

static inline LexerToken MakeError(const Lexer *lexer, const char *msg);

static inline void SkipSpaces(Lexer *lexer);

static inline bool IsAtEnd(const Lexer *lexer);

static inline char Peek(const Lexer *lexer);

static inline bool Match(Lexer *lexer, char c);

static inline char Advance(Lexer *lexer);

void Lexer_Initialize(Lexer *lexer, const char *src)
{
	lexer->cur = src;
	lexer->curLine = 1;
	lexer->curColumn = 1;
}

LexerToken Lexer_Next(Lexer *lexer)
{
	SkipSpaces(lexer);

	lexer->start = lexer->cur;
	lexer->startLine = lexer->curLine;
	lexer->startColumn = lexer->curColumn;

	if (IsAtEnd(lexer))
		return MakeToken(lexer, LEXER_TOKEN_EOF);

	char c = Advance(lexer);
	if (isalpha(c) || c == '_')
		return Identifier(lexer);
	if (c == '0' && Match(lexer, 'x'))
		return Hexadecimal(lexer);
	if (isdigit(c))
		return Number(lexer, false);

	switch (c) {
	case '"': return String(lexer);
	case '.':
		if (isdigit(Peek(lexer)))
			return Number(lexer, true);
		return MakeToken(lexer, LEXER_TOKEN_DOT);
	case ',': return MakeToken(lexer, LEXER_TOKEN_COMMA);
	case ':': return MakeToken(lexer, LEXER_TOKEN_COLON);
	case ';': return MakeToken(lexer, LEXER_TOKEN_SEMICOLON);
	}

	return MakeError(lexer, "Invalid token");
}

LexerToken Identifier(Lexer *lexer)
{
	while (isalnum(Peek(lexer)) || Peek(lexer) == '_') {
		Advance(lexer);
	}

	return MakeToken(lexer, LEXER_TOKEN_IDENTIFIER);
}

LexerToken String(Lexer *lexer)
{
	while (Peek(lexer) != '"' && !IsAtEnd(lexer))
	{
		Advance(lexer);
	}

	if (IsAtEnd(lexer))
		return MakeError(lexer, "Unexpected EOF, expected \"");

	Advance(lexer);
	return MakeToken(lexer, LEXER_TOKEN_STRING_LITERAL);
}

LexerToken Hexadecimal(Lexer *lexer)
{
	if (!isxdigit(Advance(lexer)))
		return MakeError(lexer, "Expected a valid hexadecimal character");

	while (isxdigit(Peek(lexer))) {
		Advance(lexer);
	}

	return MakeToken(lexer, LEXER_TOKEN_HEX_LITERAL);
}

LexerToken Number(Lexer *lexer, bool isFloat)
{
	while (isdigit(Peek(lexer))) {
		Advance(lexer);
	}

	if (Peek(lexer) == '.' && !isFloat) {
		isFloat = true;
		Advance(lexer);

		while (isdigit(Peek(lexer))) {
			Advance(lexer);
		}
	}

	if (Match(lexer, 'f') || isFloat)
		return MakeToken(lexer, LEXER_TOKEN_FLOAT_LITERAL);
	return MakeToken(lexer, LEXER_TOKEN_INT_LITERAL);
}

inline LexerToken MakeToken(const Lexer *lexer, LexerTokenType type)
{
	LexerToken token = {
		type,
		lexer->startLine,
		lexer->startColumn,
		(u32)(lexer->cur - lexer->start),
		lexer->start
	};
	return token;
}

inline LexerToken MakeError(const Lexer *lexer, const char *msg)
{
	LexerToken token = {
		LEXER_TOKEN_ERROR,
		lexer->startLine,
		lexer->startColumn,
		strlen(msg),
		msg
	};
	return token;
}

inline void SkipSpaces(Lexer *lexer)
{
	while (true) {
		switch (Peek(lexer)) {
		case '\n':
		case ' ':
		case '\t':
			Advance(lexer);
			break;
		default:
			return;
		}
	}
}

inline bool IsAtEnd(const Lexer *lexer)
{
	return *lexer->cur == '\0';
}

inline char Peek(const Lexer *lexer)
{
	return *lexer->cur;
}

inline bool Match(Lexer *lexer, char c)
{
	if (IsAtEnd(lexer))
		return false;
	if (*lexer->cur != c)
		return false;
	Advance(lexer);
	return true;
}

inline char Advance(Lexer *lexer)
{
	if (*lexer->cur == '\n') {
		++lexer->curLine;
		lexer->curColumn = 0;
	}
	++lexer->curColumn;
	return *(lexer->cur++);
}
