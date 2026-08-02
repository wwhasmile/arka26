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

static void ReportError(AnalyzerState *state, const char *msg, u32 id);

static void Root(AnalyzerState *state);

GfxcAstAnnotation *GFXC_Analyze(const GfxcAstNode *ast)
{
	AnalyzerState state = {
		ast,
		malloc(Stack_Count(ast) * sizeof(GfxcAstAnnotation)),
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

static bool IsSymbol(const AnalyzerState *state, const char *id, u32 idLength, u32 *symbol);
static bool CheckFieldName(const AnalyzerState *state, u32 idx, const char *id);
static GfxcType ResolveType(AnalyzerState *state, u32 idx);

static void BuildRootSymbolTable(AnalyzerState *state);
static void Texture(AnalyzerState *state, u32 idx);
static void Region(AnalyzerState *state, u32 idx);

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
		symbol.shared.idLength = ast[i].data.dataDecl.idLength;
		symbol.shared.id = ast[i].data.dataDecl.id;
		if (IsSymbol(state, ast[i].data.dataDecl.id, ast[i].data.dataDecl.idLength, NULL)) {
			ReportError(state, "There is already a declaration with this name", i);
			continue;
		}
		switch (ast[i].type) {
		case GFXC_AST_NODE_TEXTURE:
			annotations[i].texture.id = textureCount;
			symbol.shared.type = GFXC_SYMBOL_TEXTURE;
			symbol.texture.numId = textureCount++;
			break;
		case GFXC_AST_NODE_REGION:
			annotations[i].region.id = regionCount;
			symbol.shared.type = GFXC_SYMBOL_REGION;
			symbol.region.numId = regionCount++;
			break;
		case GFXC_AST_NODE_SCRIPT:
			annotations[i].script.id = scriptCount;
			symbol.shared.type = GFXC_SYMBOL_SCRIPT;
			symbol.script.numId = scriptCount++;
			break;
		default: break; // Shouldn't get here in normal circumstances
		}
		state->symbols = Stack_Push(state->symbols, &symbol, NULL);
	}
}

void Texture(AnalyzerState *state, u32 idx)
{
	bool foundPath = false;
	bool foundHasData = false;
	bool foundFormat = false;
	bool foundWidth = false;
	bool foundHeight = false;

	const GfxcAstNode *ast = state->ast;
	GfxcAstAnnotation *annotations = state->astAnnotations;
	for (u32 i = idx + 1; i != 0; i = ast[i].next) {
		if (CheckFieldName(state, i, "path")) {
			if (foundPath) {
				ReportError(state, "Texture path has already been defined", i);
				continue;
			}
			foundPath = true;
			annotations[i].textureKey.field = GFXC_TEXTURE_FIELD_PATH;

			if (ResolveType(state, i + 1) != GFXC_TYPE_STRING)
				ReportError(state, "Texture's path only accepts string", i + 1);
			continue;
		}
		if (CheckFieldName(state, i, "hasData")) {
			if (foundHasData) {
				ReportError(state, "Texture having data has already been defined", i);
				continue;
			}
			foundHasData = true;
			annotations[i].textureKey.field = GFXC_TEXTURE_FIELD_HAS_DATA;

			if (ResolveType(state, i + 1) != GFXC_TYPE_BOOL)
				ReportError(state,
					"Texture's hasData only accepts true or false",
					i + 1);
			continue;
		}
		if (CheckFieldName(state, i, "format")) {
			if (foundFormat) {
				ReportError(state, "Texture format has already been defined", i);
				continue;
			}
			foundFormat = true;
			annotations[i].textureKey.field = GFXC_TEXTURE_FIELD_FORMAT;

			if (ResolveType(state, i + 1) != GFXC_TYPE_INT)
				ReportError(state, "Texture format only accepts integers", i + 1);
			continue;
		}
		if (CheckFieldName(state, i, "width")) {
			if (foundWidth) {
				ReportError(state, "Texture width has already been defined", i);
				continue;
			}
			foundWidth = true;
			annotations[i].textureKey.field = GFXC_TEXTURE_FIELD_WIDTH;

			if (ResolveType(state, i + 1) != GFXC_TYPE_INT)
				ReportError(state, "Texture width only accepts integers", i + 1);
			continue;
		}
		if (CheckFieldName(state, i, "height")) {
			if (foundHeight) {
				ReportError(state, "Texture height has already been defined", i);
				continue;
			}
			foundHeight = true;
			annotations[i].textureKey.field = GFXC_TEXTURE_FIELD_HEIGHT;

			if (ResolveType(state, i + 1) != GFXC_TYPE_INT)
				ReportError(state, "Texture height only accepts integers", i + 1);
			continue;
		}
	}

	if (!foundPath)
		ReportError(state, "Texture always expects path", idx);
}

void Region(AnalyzerState *state, u32 idx)
{
	bool foundTexture = false;
	bool foundX = false;
	bool foundY = false;
	bool foundWidth = false;
	bool foundHeight = false;

	const GfxcAstNode *ast = state->ast;
	GfxcAstAnnotation *annotations = state->astAnnotations;
	for (u32 i = idx + 1; i != 0; i = ast[i].next) {
		if (CheckFieldName(state, i, "texture")) {
			if (foundTexture) {
				ReportError(state, "Region texture has already been defined", i);
				continue;
			}
			foundTexture = true;
			annotations[i].regionKey.field = GFXC_REGION_FIELD_TEXTURE;

			if (ResolveType(state, i + 1) != GFXC_TYPE_TEXTURE)
				ReportError(state, "Invalid region texture", i + 1);
			continue;
		}
		if (CheckFieldName(state, i, "x")) {
			if (foundX) {
				ReportError(state, "Region X has already been defined", i);
				continue;
			}
			foundX = true;
			annotations[i].regionKey.field = GFXC_REGION_FIELD_X;

			if ((ResolveType(state, i + 1) & GFXC_TYPE_FLOAT) == 0)
				ReportError(state,
					"Region X only accepts floating point values",
					i + 1);
			continue;
		}
		if (CheckFieldName(state, i, "y")) {
			if (foundY) {
				ReportError(state, "Region Y has already been defined", i);
				continue;
			}
			foundY = true;
			annotations[i].regionKey.field = GFXC_REGION_FIELD_Y;

			if ((ResolveType(state, i + 1) & GFXC_TYPE_FLOAT) == 0)
				ReportError(state,
					"Region Y only accepts floating point values",
					i + 1);
			continue;
		}
		if (CheckFieldName(state, i, "width")) {
			if (foundWidth) {
				ReportError(state, "Region width has already been defined", i);
				continue;
			}
			foundWidth = true;
			annotations[i].regionKey.field = GFXC_REGION_FIELD_WIDTH;

			if ((ResolveType(state, i + 1) & GFXC_TYPE_FLOAT) == 0)
				ReportError(state,
					"Region width only accepts floating point values",
					i + 1);
			continue;
		}
		if (CheckFieldName(state, i, "height")) {
			if (foundHeight) {
				ReportError(state, "Region height has already been defined", i);
				continue;
			}
			foundHeight = true;
			annotations[i].regionKey.field = GFXC_REGION_FIELD_HEIGHT;

			if ((ResolveType(state, i + 1) & GFXC_TYPE_FLOAT) == 0)
				ReportError(state,
					"Region height only accepts floating point values",
					i + 1);
			continue;
		}
	}

	if (!foundTexture)
		ReportError(state, "Region always expects a texture", idx);
}

bool IsSymbol(const AnalyzerState *state, const char *id, u32 idLength, u32 *symbol)
{
	const GfxcSymbol *symbols = state->symbols;
	for (u32 i = 0, j = Stack_Count(state->symbols); i < j; ++i) {
		if (idLength != symbols[i].shared.idLength)
			continue;
		if (strncmp(symbols[i].shared.id, id, idLength) != 0)
			continue;
		if (symbol != NULL)
			*symbol = i;
		return true;
	}
	return false;
}

bool CheckFieldName(const AnalyzerState *state, u32 idx, const char *id)
{
	const GfxcAstNode *ast = state->ast;
	return strncmp(ast[idx].data.field.id, id, ast[idx].data.field.idLength) == 0;
}

static GfxcType ResolveIdentifier(AnalyzerState *state, u32 idx);

GfxcType ResolveType(AnalyzerState *state, u32 idx)
{
	const GfxcAstNode *ast = state->ast;
	GfxcAstAnnotation *annotations = state->astAnnotations;

	switch (ast[idx].type) {
	case GFXC_AST_NODE_IDENTIFIER:
		return ResolveIdentifier(state, idx);
	case GFXC_AST_NODE_INT_LITERAL:
		annotations[idx].value.shared.type = GFXC_TYPE_INT;
		annotations[idx].value.integer.value = ast[idx].data.intLiteral.value;
		return GFXC_TYPE_INT;
	case GFXC_AST_NODE_BOOL_LITERAL:
		annotations[idx].value.shared.type = GFXC_TYPE_BOOL;
		annotations[idx].value.boolv.value = ast[idx].data.boolLiteral.value;
		return GFXC_TYPE_BOOL;
	case GFXC_AST_NODE_FLOAT_LITERAL:
		annotations[idx].value.shared.type = GFXC_TYPE_FLOAT;
		annotations[idx].value.floatv.value = ast[idx].data.floatLiteral.value;
		return GFXC_TYPE_FLOAT;
	case GFXC_AST_NODE_HEX_LITERAL:
		annotations[idx].value.shared.type = GFXC_TYPE_HEX;
		annotations[idx].value.hex.value = ast[idx].data.hexLiteral.value;
		return GFXC_TYPE_HEX;
	case GFXC_AST_NODE_STRING_LITERAL:
		annotations[idx].value.shared.type = GFXC_TYPE_STRING;
		annotations[idx].value.str.data = ast[idx].data.stringLiteral.data;
		annotations[idx].value.str.dataLength = ast[idx].data.stringLiteral.dataLength;
		return GFXC_TYPE_STRING;
	default: return GFXC_TYPE_NONE;
	}
}

GfxcType ResolveIdentifier(AnalyzerState *state, u32 idx)
{
	const GfxcAstNode *ast = state->ast;
	GfxcAstAnnotation *annotations = state->astAnnotations;
	const GfxcSymbol *symbols = state->symbols;

	u32 symbol;
	if (!IsSymbol(state, ast[idx].data.identifier.id, ast[idx].data.identifier.idLength, &symbol))
		return GFXC_TYPE_NONE;
	switch (symbols[symbol].shared.type) {
	case GFXC_SYMBOL_CONSTANT:
		annotations[idx].value = symbols[symbol].constant.value;
		return symbols[symbol].constant.value.shared.type;
	case GFXC_SYMBOL_TEXTURE:
		annotations[idx].value.shared.type = GFXC_TYPE_TEXTURE;
		annotations[idx].value.texture.id = symbols[symbol].texture.numId;
		return GFXC_TYPE_TEXTURE;
	case GFXC_SYMBOL_REGION:
		annotations[idx].value.shared.type = GFXC_TYPE_REGION;
		annotations[idx].value.region.id = symbols[symbol].region.numId;
		return GFXC_TYPE_REGION;
	case GFXC_SYMBOL_SCRIPT:
		annotations[idx].value.shared.type = GFXC_TYPE_SCRIPT;
		annotations[idx].value.script.id = symbols[symbol].script.numId;
		return GFXC_TYPE_SCRIPT;
	case GFXC_SYMBOL_LABEL:
		annotations[idx].value.shared.type = GFXC_TYPE_LABEL;
		annotations[idx].value.label.to = symbols[symbol].label.to;
		return GFXC_TYPE_LABEL;
	case GFXC_SYMBOL_REG:
		annotations[idx].value.shared.type = symbols[symbol].reg.regType;
		annotations[idx].value.reg.id = symbols[symbol].reg.idx;
		return symbols[symbol].reg.regType;
	default: return GFXC_TYPE_NONE;
	}
}

void ReportError(AnalyzerState *state, const char *msg, u32 id)
{
	const GfxcAstNode *ast = state->ast;
	state->error = true;
	GFXC_Error(msg, ast[id].line, ast[id].column, ast[0].data.head.src);
}
