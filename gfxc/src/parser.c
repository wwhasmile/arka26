#include <gfxc.h>

#include <stack.h>
#include <lexer.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PARSER_DEFAULT_GFXC_AST_CAPACITY 128

static const char *GFXC_TEXTURE_BLOCK_KEYWORD = "texture";
static const char *GFXC_REGION_BLOCK_KEYWORD = "region";
static const char *GFXC_SCRIPT_BLOCK_KEYWORD = "script";
static const char *GFXC_END_BLOCK_KEYWORD = "end";

typedef struct {
	Lexer lexer;
	LexerToken token;
	GfxcAstNode *ast;
	bool error;
} ParserState;

static void Root(ParserState *state);

static u32 Push(ParserState *state, GfxcAstNodeType type, u32 last);

static inline bool IsKeyword(const ParserState *state, const char *keyword);
static inline bool Match(ParserState *state, LexerTokenType type, LexerToken *token);
static inline bool Check(const ParserState *state, LexerTokenType type, LexerToken *token);
static inline bool IsAtEnd(const ParserState *state);
static inline LexerToken Advance(ParserState *state);

static void ReportError(ParserState *state, const char *message);

GfxcAstNode *GFXC_Parse(const char *src)
{
	ParserState state = { 0 };
	Lexer_Initialize(&state.lexer, src);
	Push(&state, GFXC_AST_NODE_HEAD, 0);
	state.ast[0].data.head.src = src;
	Advance(&state);

	Root(&state);

	if (state.error && state.ast != NULL) {
		Stack_Release(state.ast);
		return NULL;
	}

	state.ast[0].data.head.count = Stack_Count(state.ast);
	return state.ast;
}

static u32 Identifier(ParserState *state, u32 last);

static u32 DataDeclaration(ParserState *state, GfxcAstNodeType type, u32 last);
static u32 ScriptDeclaration(ParserState *state, u32 last);

static u32 Value(ParserState *state, u32 last);

void Root(ParserState *state)
{
	u32 id = 0;
	while (!IsAtEnd(state)) {
		if (IsKeyword(state, GFXC_TEXTURE_BLOCK_KEYWORD)) {
			id = DataDeclaration(state, GFXC_AST_NODE_TEXTURE, id);
			continue;
		}
		if (IsKeyword(state, GFXC_REGION_BLOCK_KEYWORD)) {
			id = DataDeclaration(state, GFXC_AST_NODE_REGION, id);
			continue;
		}
		if (IsKeyword(state, GFXC_SCRIPT_BLOCK_KEYWORD)) {
			id = ScriptDeclaration(state, id);
			continue;
		}
		ReportError(state, "Not a texture declaration, region definition, or a script");
	}
}

u32 Identifier(ParserState *state, u32 last)
{
	u32 id = Push(state, GFXC_AST_NODE_IDENTIFIER, last);
	state->ast[id].data.identifier.idLength = state->token.length;
	state->ast[id].data.identifier.id = state->token.lexeme;
	return id;
}

static u32 Field(ParserState *state, u32 last);

u32 DataDeclaration(ParserState *state, GfxcAstNodeType type, u32 last)
{
	u32 id = Push(state, type, last);
	Advance(state);

	LexerToken idToken;
	if (!Match(state, LEXER_TOKEN_IDENTIFIER, &idToken)) {
		ReportError(state, "Expected data block identifier");
		return 0;
	}
	state->ast[id].data.decl.idLength = idToken.length;
	state->ast[id].data.decl.id = idToken.lexeme;

	u32 fieldId = 0;
	while (!IsAtEnd(state) && !IsKeyword(state, GFXC_END_BLOCK_KEYWORD)) {
		fieldId = Field(state, fieldId);
	}

	if (IsAtEnd(state)) {
		ReportError(state, "Expected end, got EOF instead");
		return 0;
	}
	Advance(state);

	return id;
}

static u32 Statement(ParserState *state, u32 last);

u32 ScriptDeclaration(ParserState *state, u32 last)
{
	u32 id = Push(state, GFXC_AST_NODE_SCRIPT, last);
	Advance(state);

	LexerToken idToken;
	if (!Match(state, LEXER_TOKEN_IDENTIFIER, &idToken)) {
		ReportError(state, "Expected script identifier");
		return 0;
	}
	state->ast[id].data.decl.idLength = idToken.length;
	state->ast[id].data.decl.id = idToken.lexeme;

	u32 statementId = 0;
	while (!IsAtEnd(state) && !IsKeyword(state, GFXC_END_BLOCK_KEYWORD)) {
		statementId = Statement(state, statementId);
	}

	if (IsAtEnd(state)) {
		ReportError(state, "Expected end, got EOF instead");
		return 0;
	}
	Advance(state);

	return id;
}

u32 Field(ParserState *state, u32 last)
{
	LexerToken idToken;
	if (!Match(state, LEXER_TOKEN_IDENTIFIER, &idToken)) {
		ReportError(state, "Expected field identifier");
		return 0;
	}

	u32 id = Push(state, GFXC_AST_NODE_FIELD, last);
	state->ast[id].data.field.idLength = idToken.length;
	state->ast[id].data.field.id = idToken.lexeme;

	u32 valueId = Value(state, 0);
	if (valueId == 0) {
		return 0;
	}

	if (!Match(state, LEXER_TOKEN_SEMICOLON, &idToken)) {
		ReportError(state, "Expected ;");
		return 0;
	}

	return id;
}

static u32 IdentifierStatement(ParserState *state, u32 last);
static u32 TimeLabel(ParserState *state, bool relative, u32 last);

u32 Statement(ParserState *state, u32 last)
{
	switch (state->token.type) {
	case LEXER_TOKEN_IDENTIFIER:
		return IdentifierStatement(state, last);
	case LEXER_TOKEN_PLUS:
		Advance(state);
		return TimeLabel(state, true, last);
	case LEXER_TOKEN_FLOAT_LITERAL: /* FALLTHROUGH */
	case LEXER_TOKEN_INT_LITERAL:
		return TimeLabel(state, false, last);
	default:
		ReportError(state, "Invalid statement");
		return 0;
	}
}

u32 IdentifierStatement(ParserState *state, u32 last)
{
	u32 id = Push(state, GFXC_AST_NODE_NONE, last);
	state->ast[id].data.identifier.idLength = state->token.length;
	state->ast[id].data.identifier.id = state->token.lexeme;
	Advance(state);

	if (Match(state, LEXER_TOKEN_COLON, NULL)) {
		state->ast[id].type = GFXC_AST_NODE_LABEL;
		return id;
	}

	state->ast[id].type = GFXC_AST_NODE_INSTRUCTION;
	u32 valueId = 0;
	while (true) {
		valueId = Value(state, valueId);
		if (Match(state, LEXER_TOKEN_SEMICOLON, NULL))
			break;
		if (!Match(state, LEXER_TOKEN_COMMA, NULL)) {
			ReportError(state, "Expected comma after operand, or semicolon if it's end of instruction");
			return 0;
		}
	}

	return id;
}

u32 TimeLabel(ParserState *state, bool relative, u32 last)
{
	u32 id = Push(state, GFXC_AST_NODE_TIME_LABEL, last);
	state->ast[id].data.timeLabel.relative = relative;
	char *endPtr;
	state->ast[id].data.timeLabel.offset = strtof(state->token.lexeme, &endPtr);

	Advance(state);
	if (!Match(state, LEXER_TOKEN_COLON, NULL)) {
		ReportError(state, "Expected ':'");
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
		if (IsKeyword(state, "true")) {
			id = Bool(state, true, last);
			break;
		}
		if (IsKeyword(state, "false")) {
			id = Bool(state, false, last);
			break;
		}
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
		ReportError(state, "Expected a valid value");
		return 0;
	}

	Advance(state);
	return id;
}

u32 Hex(ParserState *state, u32 last)
{
	u32 id = Push(state, GFXC_AST_NODE_HEX_LITERAL, last);
	char *endPtr;
	state->ast[id].data.hexLiteral.value = (u32)strtoul(state->token.lexeme, &endPtr, 16);
	return id;
}

u32 Integer(ParserState *state, u32 last, bool negative)
{
	u32 id = Push(state, GFXC_AST_NODE_INT_LITERAL, last);
	char *endPtr;
	state->ast[id].data.intLiteral.value = strtol(state->token.lexeme, &endPtr, 10);
	if (negative)
		state->ast[id].data.intLiteral.value *= -1;
	return id;
}

u32 Float(ParserState *state, u32 last, bool negative)
{
	u32 id = Push(state, GFXC_AST_NODE_FLOAT_LITERAL, last);
	char *endPtr;
	state->ast[id].data.floatLiteral.value = strtof(state->token.lexeme, &endPtr);
	if (negative)
		state->ast[id].data.floatLiteral.value *= -1;
	return id;
}

u32 String(ParserState *state, u32 last)
{
	u32 id = Push(state, GFXC_AST_NODE_STRING_LITERAL, last);
	state->ast[id].data.stringLiteral.dataLength = state->token.length - 2;
	state->ast[id].data.stringLiteral.data = state->token.lexeme + 1;
	return id;
}

u32 Bool(ParserState *state, bool value, u32 last)
{
	u32 id = Push(state, GFXC_AST_NODE_BOOL_LITERAL, last);
	state->ast[id].data.boolLiteral.value = value;
	return id;
}

u32 Push(ParserState *state, GfxcAstNodeType type, u32 last)
{
	if (state->ast == NULL)
		state->ast = Stack_InitCapacity(sizeof(GfxcAstNode), PARSER_DEFAULT_GFXC_AST_CAPACITY);

	GfxcAstNode node = { 0 };
	node.type = type;
	node.line = state->token.line;
	node.column = state->token.column;
	node.next = 0;

	u32 idx;
	state->ast = Stack_Push(state->ast, &node, &idx);
	if (last > 0)
		state->ast[last].next = idx;
	return idx;
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

void ReportError(ParserState *state, const char *message)
{
	GFXC_Error(message, state->token.line, state->token.column, state->ast[0].data.head.src);
	state->error = true;
	while (!IsAtEnd(state) && !IsKeyword(state, GFXC_TEXTURE_BLOCK_KEYWORD) &&
			!IsKeyword(state, GFXC_REGION_BLOCK_KEYWORD) &&
			!IsKeyword(state, GFXC_SCRIPT_BLOCK_KEYWORD)) {
		Advance(state);
	}
}
