#include "lexer.h"

#include <string.h>

static inline LexerToken MakeToken(const Lexer *lexer, LexerTokenType type);

static inline LexerToken MakeError(const Lexer *lexer, const char *msg);

static inline void SkipSpaces(Lexer *lexer);

static inline bool IsAtEnd(const Lexer *lexer);

static inline char Peek(const Lexer *lexer);

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

	switch (Advance(lexer)) {
	case '.': return MakeToken(lexer, LEXER_TOKEN_DOT);
	case ',': return MakeToken(lexer, LEXER_TOKEN_COMMA);
	case ':': return MakeToken(lexer, LEXER_TOKEN_COLON);
	case ';': return MakeToken(lexer, LEXER_TOKEN_SEMICOLON);
	case '\0': return MakeToken(lexer, LEXER_TOKEN_EOF);
	default:
		return MakeError(lexer, "Invalid token");
	}
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
			++lexer->curLine;
			lexer->curColumn = 0;
			/* FALLTHROUGH */
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

inline char Advance(Lexer *lexer)
{
	++lexer->curColumn;
	return *(lexer->cur++);
}
