#include <gfxc.h>

typedef struct {
	const GfxcAstNode *ast;
	GfxcAstNodeSemanticData *semanticData;
	bool error;
} AnalyzerState;

GfxcAstNodeSemanticData *GFXC_AnalyzeAST(const GfxcAstNode *ast)
{
	return NULL;
}
