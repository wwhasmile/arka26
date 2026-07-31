#include <gfxc.h>

typedef struct {
	const GfxcAstNode *ast;
	GfxcAstNodeSemanticData *semanticData;
	bool error;
} AnalyzerState;

GfxcAstNodeSemanticData *GFXC_AnalyzeAST(const GfxcAstNode *ast)
{
	AnalyzerState state = {
		ast,
		NULL,
		false
	};

	return NULL;
}
