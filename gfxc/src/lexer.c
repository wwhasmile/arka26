#include "lexer.h"

void Lexer_Initialize(Lexer *lexer, const char *src)
{
	lexer->cur = src;
	lexer->curLine = 1;
	lexer->curColumn = 1;
}
