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

extern const GfxcSymbol GFXC_DEFAULT_SYMBOLS[];
extern const GfxcInstruction GFXC_INSTRUCTIONS[];

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

static const GfxcSymbol *GetSymbol(const AnalyzerState *state, const char *id, u32 idLength);
static bool CheckFieldName(const AnalyzerState *state, u32 idx, const char *id);
static GfxcType ResolveType(AnalyzerState *state, u32 idx);

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
		symbol.idLength = ast[i].data.decl.idLength;
		symbol.id = ast[i].data.decl.id;
		if (GetSymbol(state, ast[i].data.decl.id, ast[i].data.decl.idLength) != NULL) {
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
		ReportError(state, "Invalid texture attribute", i);
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

			if ((ResolveType(state, i + 1) & GFXC_TYPE_NUMBER) == 0)
				ReportError(state,
					"Region X only accepts numbers",
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

			if ((ResolveType(state, i + 1) & GFXC_TYPE_NUMBER) == 0)
				ReportError(state,
					"Region Y only accepts numbers",
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

			if ((ResolveType(state, i + 1) & GFXC_TYPE_NUMBER) == 0)
				ReportError(state,
					"Region width only accepts numbers",
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

			if ((ResolveType(state, i + 1) & GFXC_TYPE_NUMBER) == 0)
				ReportError(state,
					"Region height only accepts numbers",
					i + 1);
			continue;
		}
		ReportError(state, "Invalid region attribute", i);
	}

	if (!foundTexture)
		ReportError(state, "Region always expects a texture", idx);
}

static void BuildScriptSymbolTable(AnalyzerState *state, u32 idx);
static void Instuction(AnalyzerState *state, u32 idx);

void Script(AnalyzerState *state, u32 idx)
{
	u32 symbolResetMarker = Stack_Count(state->symbols);
	BuildScriptSymbolTable(state, idx);

	const GfxcAstNode *ast = state->ast;
	for (u32 i = idx + 1; i != 0; i = ast[i].next) {
		if (ast[i].type != GFXC_AST_NODE_INSTRUCTION)
			continue;
		Instuction(state, i);
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
		annotations[i].value.label.to = inst;
		annotations[i].value.label.type = GFXC_TYPE_LABEL;
	}

	state->symbols = symbols;
}

void Instuction(AnalyzerState *state, u32 idx) {
	const GfxcAstNode *ast = state->ast;

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

	GfxcAstAnnotation *annotations = state->astAnnotations;
	annotations[idx].instruction.id = instruction->id;

	u32 arg = 0;
	for (i = idx + 1; i != 0; i = ast[i].next) {
		if (arg >= instruction->argc) {
			ReportError(state, "Too many arguments", idx);
			return;
		}

		GfxcType type = ResolveType(state, i);
		GfxcType argType = instruction->argt[arg];
		if ((argType & GFXC_TYPE_NUMBER) == GFXC_TYPE_NUMBER) {
			if ((type & GFXC_TYPE_NUMBER) == 0)
				ReportError(state, "Expected a number", i);
		} else if ((argType & GFXC_TYPE_FLOAT) == GFXC_TYPE_FLOAT) {
			if ((type & GFXC_TYPE_NUMBER) == 0 )
				ReportError(state, "Expected a floating point", i);
		} else if ((argType & GFXC_TYPE_INT) == GFXC_TYPE_INT) {
			if ((type & GFXC_TYPE_INT) != GFXC_TYPE_INT)
				ReportError(state, "Expected an integer", i);
		} else if ((argType & GFXC_TYPE_HEX) == GFXC_TYPE_HEX) {
			if ((type & GFXC_TYPE_HEX) == 0)
				ReportError(state, "Expected a hexadecimal", i);
		} else if ((argType & GFXC_TYPE_BOOL) == GFXC_TYPE_BOOL) {
			if ((type & GFXC_TYPE_BOOL) == 0)
				ReportError(state, "Expected a boolean", i);
		} else if ((argType & GFXC_TYPE_TEXTURE) == GFXC_TYPE_TEXTURE) {
			if ((type & GFXC_TYPE_TEXTURE) == 0)
				ReportError(state, "Expected a texture", i);
		} else if ((argType & GFXC_TYPE_REGION) == GFXC_TYPE_REGION) {
			if ((type & GFXC_TYPE_REGION) == 0)
				ReportError(state, "Expected a region", i);
		} else if ((argType & GFXC_TYPE_SCRIPT) == GFXC_TYPE_SCRIPT) {
			if ((type & GFXC_TYPE_SCRIPT) == 0)
				ReportError(state, "Expected a script", i);
		} else if ((argType & GFXC_TYPE_LABEL) == GFXC_TYPE_LABEL) {
			if ((type & GFXC_TYPE_LABEL) == 0)
				ReportError(state, "Expected a label", i);
		}
		if ((argType & GFXC_TYPE_RW_REGISTER) == GFXC_TYPE_RW_REGISTER) {
			if ((type & GFXC_TYPE_RW_REGISTER) != GFXC_TYPE_RW_REGISTER)
				ReportError(state, "Expected a writeable register", i);
		} else if ((argType & GFXC_TYPE_R_REGISTER) == GFXC_TYPE_R_REGISTER) {
			if ((type & GFXC_TYPE_R_REGISTER) == 0)
				ReportError(state, "Expected a register", i);
		}
		arg++;
	}
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

bool CheckFieldName(const AnalyzerState *state, u32 idx, const char *id)
{
	const GfxcAstNode *ast = state->ast;
	u32 len = strlen(id);
	if (len != ast[idx].data.field.idLength)
		return false;
	return strncmp(id, ast[idx].data.field.id, ast[idx].data.field.idLength) == 0;
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

	const GfxcSymbol *symbol = GetSymbol(state, ast[idx].data.identifier.id, ast[idx].data.identifier.idLength);
	if (symbol == NULL)
		return GFXC_TYPE_NONE;
	annotations[idx].value = symbol->value;
	return symbol->value.shared.type;
}

void ReportError(AnalyzerState *state, const char *msg, u32 id)
{
	const GfxcAstNode *ast = state->ast;
	state->error = true;
	GFXC_Error(msg, ast[id].line, ast[id].column, ast[0].data.head.src);
}
