#include <gfxc.h>

#include <stack.h>

#include <stdlib.h>
#include <string.h>

typedef struct {
	const GfxcAstNode *ast;
	GfxcAstAnnotation *astAnnotations;
	GfxcSymbol *symbols;
	bool error;
} AnalyzerState;

static const char *GFXC_INVALID_TYPE_MESSAGES[] = {
	NULL,
	"Expected a texture id",
	"Expected a region id",
	"Expected a script id",
	"Expected a hexadecimal number",
	"Expected an integer",
	"Expected a boolean (true or false)",
	"Expected a float",
	"Expected a jump label",
	"Expected an int register x[0,7]",
	"Expected a float register f[0,7]",
	"Expected a string",
};
static const char *GFXC_TEXTURE_REQUIRED_MESSAGE[] = {
	"Path is required for any texture",
};
static const char *GFXC_REGION_REQUIRED_MESSAGE[] = {
	"Texture id for is required for any region",
};

static void ReportError(AnalyzerState *state, const char *msg, u32 id);

static void Root(AnalyzerState *state);

GfxcAstAnnotation *GFXC_Analyze(const GfxcAstNode *ast)
{
	AnalyzerState state = {
		ast,
		calloc(Stack_Count(ast), sizeof(GfxcAstAnnotation)),
		Stack_InitCapacity(sizeof(GfxcSymbol), 16),
		false
	};

	Root(&state);

	Stack_Release(state.symbols);

	if (state.error)
	{
		free(state.astAnnotations);
		return NULL;
	}

	return state.astAnnotations;
}

static const GfxcSymbol *GetSymbol(const AnalyzerState *state, const char *id, u32 idLength);
static void Attribute(AnalyzerState *state, u32 idx, const GfxcAttribute *attributes, bool *found, u32 count);
static bool MatchType(AnalyzerState *state, u32 idx, GfxcType expected);

static void BuildRootSymbolTable(AnalyzerState *state);
static void Texture(AnalyzerState *state, u32 idx);
static void Region(AnalyzerState *state, u32 idx);
static void Script(AnalyzerState *state, u32 idx);

void Root(AnalyzerState *state)
{
	BuildRootSymbolTable(state);

	const GfxcAstNode *ast = state->ast;
	for (u32 i = 1; i != 0; i = ast[i].next) {
		switch (ast[i].type) {
		case GFXC_AST_NODE_TEXTURE:
			Texture(state, i);
			break;
		case GFXC_AST_NODE_REGION:
			Region(state, i);
			break;
		case GFXC_AST_NODE_SCRIPT:
			Script(state, i);
			break;
		default: break; // Shouldn't get here in normal circumstances
		}
	}
}

void BuildRootSymbolTable(AnalyzerState *state)
{
	u32 textureCount = 0;
	u32 regionCount = 0;
	u32 scriptCount = 0;

	const GfxcAstNode *ast = state->ast;
	GfxcAstAnnotation *annotations = state->astAnnotations;
	for (u32 i = 1; i != 0; i = ast[i].next) {
		GfxcSymbol symbol = { 0 };
		symbol.idLength = ast[i].data.attrSet.idLength;
		symbol.id = ast[i].data.attrSet.id;
		if (GetSymbol(state, ast[i].data.attrSet.id, ast[i].data.attrSet.idLength) != NULL) {
			ReportError(state, "There is already a declaration with this name", i);
			continue;
		}
		switch (ast[i].type) {
		case GFXC_AST_NODE_TEXTURE:
			annotations[i].texture.id = textureCount;
			symbol.value.shared.type = GFXC_TYPE_TEXTURE;
			symbol.value.texture.id = textureCount++;
			break;
		case GFXC_AST_NODE_REGION:
			annotations[i].region.id = regionCount;
			symbol.value.shared.type = GFXC_TYPE_REGION;
			symbol.value.region.id = regionCount++;
			break;
		case GFXC_AST_NODE_SCRIPT:
			annotations[i].script.id = scriptCount;
			symbol.value.shared.type = GFXC_TYPE_SCRIPT;
			symbol.value.script.id = scriptCount++;
			break;
		default: break; // Shouldn't get here in normal circumstances
		}
		state->symbols = Stack_Push(state->symbols, &symbol, NULL);
	}
}

void Texture(AnalyzerState *state, u32 idx)
{
	bool found[ARRAY_LENGTH(GFXC_TEXTURE_ATTRIBUTES)] = { 0 };

	for (u32 i = idx + 1; i != 0; i = state->ast[i].next) {
		Attribute(state, i, GFXC_TEXTURE_ATTRIBUTES, found, ARRAY_LENGTH(GFXC_TEXTURE_ATTRIBUTES));
	}

	for (u32 i = 0; i < ARRAY_LENGTH(GFXC_TEXTURE_ATTRIBUTES); ++i) {
		if (GFXC_TEXTURE_ATTRIBUTES[i].mandatory && !found[i])
			ReportError(state, GFXC_TEXTURE_REQUIRED_MESSAGE[i], idx);
		else if (!found[i])
			state->astAnnotations[idx + 1 + i].attributeValue.data = GFXC_TEXTURE_ATTRIBUTES[i].value;
	}
}

void Region(AnalyzerState *state, u32 idx)
{
	bool found[ARRAY_LENGTH(GFXC_REGION_ATTRIBUTES)] = { 0 };

	for (u32 i = idx + 1; i != 0; i = state->ast[i].next) {
		Attribute(state, i, GFXC_REGION_ATTRIBUTES, found, ARRAY_LENGTH(GFXC_REGION_ATTRIBUTES));
	}

	for (u32 i = 0; i < ARRAY_LENGTH(GFXC_REGION_ATTRIBUTES); ++i) {
		if (GFXC_REGION_ATTRIBUTES[i].mandatory && !found[i])
			ReportError(state, GFXC_REGION_REQUIRED_MESSAGE[i], idx);
		else if (!found[i])
			state->astAnnotations[idx + 1 + i].attributeValue.data = GFXC_REGION_ATTRIBUTES[i].value;
	}
}

void Attribute(AnalyzerState *state, u32 idx, const GfxcAttribute *attributes, bool *found, u32 count)
{
	const GfxcAstNode *ast = state->ast;
	for (u32 i = 0; i < count; ++i) {
		if (ast[idx].data.attr.idLength != attributes[i].idLength)
			continue;
		if (strncmp(ast[idx].data.attr.id, attributes[i].id, attributes[i].idLength) != 0)
			continue;

		if (found[i]) {
			ReportError(state, "Attribute has already been set", idx);
			return;
		}
		found[i] = true;

		GfxcType attrt = attributes[i].value.shared.type;
		if (!MatchType(state, idx + 1, attrt))
			ReportError(state, GFXC_INVALID_TYPE_MESSAGES[attrt], idx + 1);

		state->astAnnotations[idx + 1].attributeValue.id = i;
		return;
	}
	ReportError(state, "Invalid attribute", idx);
}

static void BuildScriptSymbolTable(AnalyzerState *state, u32 idx);
static void Instuction(AnalyzerState *state, u32 idx, u32 id);

void Script(AnalyzerState *state, u32 idx)
{
	u32 symbolResetMarker = Stack_Count(state->symbols);
	BuildScriptSymbolTable(state, idx);

	const GfxcAstNode *ast = state->ast;
	for (u32 i = idx + 1, id = 0; i != 0; i = ast[i].next) {
		if (ast[i].type != GFXC_AST_NODE_INSTRUCTION)
			continue;
		Instuction(state, i, id++);
	}

	Stack_Reset(state->symbols, symbolResetMarker);
}

void BuildScriptSymbolTable(AnalyzerState *state, u32 idx)
{
	const GfxcAstNode *ast = state->ast;
	GfxcAstAnnotation *annotations = state->astAnnotations;
	GfxcSymbol *symbols = state->symbols;

	for (u32 i = idx + 1, inst = 1; i != 0; i = ast[i].next) {
		if (ast[i].type == GFXC_AST_NODE_INSTRUCTION) {
			++inst;
			continue;
		}
		if (ast[i].type != GFXC_AST_NODE_LABEL)
			continue;
		if (GetSymbol(state, ast[i].data.label.id, ast[i].data.label.idLength) != NULL) {
			ReportError(state, "There is already a symbol with this name", i);
			continue;
		}
		GfxcSymbol symbol = { 0 };
		symbol.value.label.type = GFXC_TYPE_LABEL;
		symbol.id = ast[i].data.label.id;
		symbol.idLength = ast[i].data.label.idLength;
		symbol.value.label.to = inst;
		symbols = Stack_Push(symbols, &symbol, NULL);
		annotations[i].value.data.label.to = inst;
		annotations[i].value.data.label.type = GFXC_TYPE_LABEL;
	}

	state->symbols = symbols;
}

void Instuction(AnalyzerState *state, u32 idx, u32 id) {
	const GfxcAstNode *ast = state->ast;

	state->astAnnotations[idx].instruction.id = id;

	u32 i;
	for (i = 0; GFXC_INSTRUCTIONS[i].name != NULL; ++i) {
		if (ast[idx].data.instruction.idLength != GFXC_INSTRUCTIONS[i].nameLength)
			continue;
		if (strncmp(ast[idx].data.instruction.id, GFXC_INSTRUCTIONS[i].name,
				GFXC_INSTRUCTIONS[i].nameLength) == 0)
			break;
	}

	const GfxcInstruction *instruction = GFXC_INSTRUCTIONS + i;
	if (instruction->name == NULL) {
		ReportError(state, "Instruction doesn't exist", idx);
		return;
	}
	state->astAnnotations[idx].instruction.opcode = i;

	u32 argi = 0;
	for (i = idx + 1; i != 0; i = ast[i].next) {
		if (ast[i].type == GFXC_AST_NODE_INSTRUCTION)
			break;

		if (argi >= instruction->argc) {
			ReportError(state, "Too many arguments", idx);
			return;
		}

		GfxcType argt = instruction->argt[argi];
		if (!MatchType(state, i, argt))
			ReportError(state, GFXC_INVALID_TYPE_MESSAGES[argt], i);

		argi++;
	}
	if (argi < instruction->argc)
		ReportError(state, GFXC_INVALID_TYPE_MESSAGES[instruction->argt[argi]], idx + argi + 1);
}

const GfxcSymbol *GetSymbol(const AnalyzerState *state, const char *id, u32 idLength)
{
	const GfxcSymbol *symbols = GFXC_DEFAULT_SYMBOLS;
	for (u32 i = 0; symbols[i].id != NULL; ++i) {
		if (idLength != symbols[i].idLength)
			continue;
		if (strncmp(id, symbols[i].id, idLength) != 0)
			continue;
		return symbols + i;
	}

	symbols = state->symbols;
	for (u32 i = 0, j = Stack_Count(state->symbols); i < j; ++i) {
		if (idLength != symbols[i].idLength)
			continue;
		if (strncmp(id, symbols[i].id, idLength) != 0)
			continue;
		return symbols + i;
	}
	return NULL;
}

static GfxcValue ResolveIdentifier(AnalyzerState *state, u32 idx);
static inline GfxcValue ResolveInteger(AnalyzerState *state, u32 idx);
static inline GfxcValue ResolveBool(AnalyzerState *state, u32 idx);
static inline GfxcValue ResolveFloat(AnalyzerState *state, u32 idx);
static inline GfxcValue ResolveHex(AnalyzerState *state, u32 idx);
static inline GfxcValue ResolveString(AnalyzerState *state, u32 idx);

bool MatchType(AnalyzerState *state, u32 idx, GfxcType expected)
{
	GfxcValue value;
	switch (state->ast[idx].type) {
	case GFXC_AST_NODE_IDENTIFIER:
		value = ResolveIdentifier(state, idx);
		break;
	case GFXC_AST_NODE_INT_LITERAL:
		value = ResolveInteger(state, idx);
		break;
	case GFXC_AST_NODE_BOOL_LITERAL:
		value = ResolveBool(state, idx);
		break;
	case GFXC_AST_NODE_FLOAT_LITERAL:
		value = ResolveFloat(state, idx);
		break;
	case GFXC_AST_NODE_HEX_LITERAL:
		value = ResolveHex(state, idx);
		break;
	case GFXC_AST_NODE_STRING_LITERAL:
		value = ResolveString(state, idx);
		break;
	default: return false;
	}

	state->astAnnotations[idx].value.argt = expected;
	state->astAnnotations[idx].value.data = value;
	switch (expected) {
	case GFXC_TYPE_INT:
		return value.shared.type == GFXC_TYPE_INT ||
			value.shared.type == GFXC_TYPE_HEX ||
			value.shared.type == GFXC_TYPE_BOOL ||
			value.shared.type == GFXC_TYPE_INT_REGISTER;
	case GFXC_TYPE_BOOL:
			return value.shared.type == GFXC_TYPE_BOOL ||
				value.shared.type == GFXC_TYPE_INT_REGISTER;
	case GFXC_TYPE_FLOAT:
			return value.shared.type == GFXC_TYPE_FLOAT ||
				value.shared.type == GFXC_TYPE_INT ||
				value.shared.type == GFXC_TYPE_FLOAT_REGISTER;
	default: return value.shared.type == expected;
	}
}

GfxcValue ResolveIdentifier(AnalyzerState *state, u32 idx)
{
	const GfxcAstNode *ast = state->ast;
	GfxcValue result = { 0 };

	const GfxcSymbol *symbol = GetSymbol(state, ast[idx].data.identifier.id, ast[idx].data.identifier.idLength);
	if (symbol != NULL)
		result = symbol->value;

	return result;
}

inline GfxcValue ResolveInteger(AnalyzerState *state, u32 idx)
{
	GfxcValue result = { 0 };
	result.intv.type = GFXC_TYPE_INT;
	result.intv.value = state->ast[idx].data.intLiteral.value;
	return result;
}

inline GfxcValue ResolveBool(AnalyzerState *state, u32 idx)
{
	GfxcValue result = { 0 };
	result.boolv.type = GFXC_TYPE_BOOL;
	result.boolv.value = state->ast[idx].data.boolLiteral.value;
	return result;
}

inline GfxcValue ResolveFloat(AnalyzerState *state, u32 idx)
{
	GfxcValue result = { 0 };
	result.floatv.type = GFXC_TYPE_FLOAT;
	result.floatv.value = state->ast[idx].data.floatLiteral.value;
	return result;
}

inline GfxcValue ResolveHex(AnalyzerState *state, u32 idx)
{
	GfxcValue result = { 0 };
	result.hex.type = GFXC_TYPE_HEX;
	result.hex.value = state->ast[idx].data.hexLiteral.value;
	return result;
}

inline GfxcValue ResolveString(AnalyzerState *state, u32 idx)
{
	GfxcValue result = {
		.string.type = GFXC_TYPE_STRING,
		.string.dataLength = state->ast[idx].data.stringLiteral.dataLength,
		.string.data = state->ast[idx].data.stringLiteral.data,
	};
	return result;
}

void ReportError(AnalyzerState *state, const char *msg, u32 id)
{
	const GfxcAstNode *ast = state->ast;
	state->error = true;
	GFXC_Error(msg, ast[id].line, ast[id].column, ast[0].data.head.src);
}
