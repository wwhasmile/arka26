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

static bool IsSymbol(const AnalyzerState *state, const char *id, u32 idLength);

static void BuildRootSymbolTable(AnalyzerState *state);

void Root(AnalyzerState *state)
{
	BuildRootSymbolTable(state);
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
		if (IsSymbol(state, ast[i].data.dataDecl.id, ast[i].data.dataDecl.idLength)) {
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

bool IsSymbol(const AnalyzerState *state, const char *id, u32 idLength)
{
	const GfxcSymbol *symbols = state->symbols;
	for (u32 i = 0, j = Stack_Count(state->symbols); i < j; ++i) {
		if (idLength != symbols[i].shared.idLength)
			continue;
		if (strncmp(symbols[i].shared.id, id, idLength) == 0)
			return true;
	}
	return false;
}

void ReportError(AnalyzerState *state, const char *msg, u32 id)
{
	const GfxcAstNode *ast = state->ast;
	state->error = true;
	GFXC_Error(msg, ast[id].line, ast[id].column, ast[0].data.head.src);
}
