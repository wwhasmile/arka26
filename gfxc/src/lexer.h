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

	u32 keywordCount;
	const char **keywords;
} Lexer;

#endif // LEXER_H
