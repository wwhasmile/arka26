#include "parser.h"

#include <lexer.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PARSER_DEFAULT_AST_CAPACITY 128

typedef struct {
	Lexer lexer;
	LexerToken token;
	AstNode *ast;
	u32 current;
	u32 capacity;
	bool error:1;
	bool panic:1;
} ParserState;

static void Root(ParserState *state);

static u32 Push(ParserState *state, AstNodeType type, u32 last);

static inline bool IsKeyword(const ParserState *state, const char *keyword);
static inline bool Match(ParserState *state, LexerTokenType type, LexerToken *token);
static inline bool Check(const ParserState *state, LexerTokenType type, LexerToken *token);
static inline bool IsAtEnd(const ParserState *state);
static inline LexerToken Advance(ParserState *state);

AstNode *Parser_Parse(const char *src)
{
	ParserState state = { 0 };
	Lexer_Initialize(&state.lexer, src);
	Push(&state, AST_NODE_HEAD, 0);
	Advance(&state);

	Root(&state);

	if (state.error && state.ast != NULL)
	{
		free(state.ast);
		return NULL;
	}

	return state.ast;
}

static u32 Identifier(ParserState *state, u32 last);

static u32 Texture(ParserState *state, u32 last);

static u32 Field(ParserState *state, u32 last);
static u32 Value(ParserState *state, u32 last);

void Root(ParserState *state)
{
	u32 id = 0;
	while (!IsAtEnd(state)) {
		if (IsKeyword(state, "tex"))
			id = Texture(state, id);
		else
			Advance(state);
	}
}

u32 Identifier(ParserState *state, u32 last)
{
	u32 id = Push(state, AST_NODE_IDENTIFIER, last);
	state->ast[id].data.identifier.idLength = state->token.length;
	state->ast[id].data.identifier.id = state->token.lexeme;
	return id;
}

u32 Texture(ParserState *state, u32 last)
{
	u32 id = Push(state, AST_NODE_TEXTURE, last);
	Advance(state);

	u32 fieldId = 0;
	while (!IsAtEnd(state) && !IsKeyword(state, "end")) {
		fieldId = Field(state, fieldId);
	}

	if (IsAtEnd(state))
		return 0;
	Advance(state);

	return id;
}

u32 Field(ParserState *state, u32 last)
{
	LexerToken idToken;
	if (!Match(state, LEXER_TOKEN_IDENTIFIER, &idToken)) {
		return 0;
	}

	if (!Match(state, LEXER_TOKEN_COLON, NULL)) {
		return 0;
	}

	u32 id = Push(state, AST_NODE_FIELD, last);
	state->ast[id].data.field.idLength = idToken.length;
	state->ast[id].data.field.id = idToken.lexeme;

	u32 valueId = Value(state, 0);
	if (valueId == 0) {
		return 0;
	}

	return id;
}

static u32 Hex(ParserState *state, u32 last);
static u32 Integer(ParserState *state, u32 last, bool negative);
static u32 Float(ParserState *state, u32 last, bool negative);
static u32 String(ParserState *state, u32 last);
static u32 Bool(ParserState *state, bool value, u32 last);

u32 Value(ParserState *state, u32 last)
{
	u32 id = 0;

	switch (state->token.type) {
	case LEXER_TOKEN_IDENTIFIER:
		id = Identifier(state, last);
		break;
	case LEXER_TOKEN_HEX_LITERAL:
		id = Hex(state, last);
		break;
	case LEXER_TOKEN_MINUS:
		Advance(state);
		switch (state->token.type) {
		case LEXER_TOKEN_INT_LITERAL:
			id = Integer(state, last, true);
			break;
		case LEXER_TOKEN_FLOAT_LITERAL:
			id = Float(state, last, true);
			break;
		default: return 0;
		}
		break;
	case LEXER_TOKEN_INT_LITERAL:
		id = Integer(state, last, false);
		break;
	case LEXER_TOKEN_FLOAT_LITERAL:
		id = Float(state, last, false);
		break;
	case LEXER_TOKEN_STRING_LITERAL:
		id = String(state, last);
		break;
	default:
		if (IsKeyword(state, "true")) {
			id = Bool(state, true, last);
			break;
		}
		if (IsKeyword(state, "false")) {
			id = Bool(state, false, last);
			break;
		}
		return 0;
	}

	Advance(state);
	return id;
}

u32 Hex(ParserState *state, u32 last)
{
	u32 id = Push(state, AST_NODE_HEX_LITERAL, last);
	char *endPtr;
	state->ast[id].data.hexLiteral.value = (u32)strtoul(state->token.lexeme, &endPtr, 16);
	return id;
}

u32 Integer(ParserState *state, u32 last, bool negative)
{
	u32 id = Push(state, AST_NODE_INT_LITERAL, last);
	char *endPtr;
	state->ast[id].data.intLiteral.value = (u32)strtol(state->token.lexeme, &endPtr, 10);
	if (negative)
		state->ast[id].data.intLiteral.value *= -1;
	return id;
}

u32 Float(ParserState *state, u32 last, bool negative)
{
	u32 id = Push(state, AST_NODE_FLOAT_LITERAL, last);
	char *endPtr;
	state->ast[id].data.floatLiteral.value = strtof(state->token.lexeme, &endPtr);
	if (negative)
		state->ast[id].data.floatLiteral.value *= -1;
	return id;
}

u32 String(ParserState *state, u32 last)
{
	u32 id = Push(state, AST_NODE_STRING_LITERAL, last);
	state->ast[id].data.stringLiteral.dataLength = state->token.length - 2;
	state->ast[id].data.stringLiteral.data = state->token.lexeme + 1;
	return id;
}

u32 Bool(ParserState *state, bool value, u32 last)
{
	u32 id = Push(state, AST_NODE_INT_LITERAL, last);
	state->ast[id].data.intLiteral.value = value;
	return id;
}

u32 Push(ParserState *state, AstNodeType type, u32 last)
{
	if (state->current == state->capacity) {
		u32 capacity = state->capacity == 0 ?
			PARSER_DEFAULT_AST_CAPACITY :
			state->capacity * 2;
		AstNode *ast = (AstNode *)malloc(capacity * sizeof(AstNode));
		memcpy(ast, state->ast, state->capacity * sizeof(AstNode));
		free(state->ast);
		state->ast = ast;
		state->capacity = capacity;
	}

	AstNode node = { 0 };
	node.type = type;
	node.line = state->token.line;
	node.column = state->token.column;
	node.next = 0;

	if (last > 0 && last < state->current)
		state->ast[last].next = state->current;

	state->ast[state->current++] = node;
	return state->current - 1;
}

inline bool IsKeyword(const ParserState *state, const char *keyword)
{
	LexerToken token;
	if (!Check(state, LEXER_TOKEN_IDENTIFIER, &token))
		return false;
	return strncmp(token.lexeme, keyword, token.length) == 0;
}

inline bool Match(ParserState *state, LexerTokenType type, LexerToken *token)
{
	if (!Check(state, type, token))
		return false;
	Advance(state);
	return true;
}

inline bool Check(const ParserState *state, LexerTokenType type, LexerToken *token)
{
	if (state->token.type == type && token != NULL)
		*token = state->token;
	return state->token.type == type;
}

inline bool IsAtEnd(const ParserState *state) {
	return state->token.type == LEXER_TOKEN_EOF;
}

inline LexerToken Advance(ParserState *state)
{
	LexerToken token = state->token;
	state->token = Lexer_Next(&state->lexer);
	return token;
}
