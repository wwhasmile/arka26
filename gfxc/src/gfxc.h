#ifndef GFXC_H
#define GFXC_H

#include <defines.h>

typedef enum {
	GFXC_AST_NODE_NONE,

	GFXC_AST_NODE_HEAD,

	GFXC_AST_NODE_TEXTURE,
	GFXC_AST_NODE_REGION,
	GFXC_AST_NODE_CONSTANT,

	GFXC_AST_NODE_FIELD,

	GFXC_AST_NODE_SCRIPT,
	GFXC_AST_NODE_LABEL,
	GFXC_AST_NODE_TIME_LABEL,
	GFXC_AST_NODE_INSTRUCTION,
	GFXC_AST_NODE_REGION_REGISTER,
	GFXC_AST_NODE_INT_REGISTER,
	GFXC_AST_NODE_FLOAT_REGISTER,

	GFXC_AST_NODE_IDENTIFIER,
	GFXC_AST_NODE_INT_LITERAL,
	GFXC_AST_NODE_BOOL_LITERAL,
	GFXC_AST_NODE_FLOAT_LITERAL,
	GFXC_AST_NODE_HEX_LITERAL,
	GFXC_AST_NODE_STRING_LITERAL,

	GFXC_AST_NODE_ENUM_COUNT
} GfxcAstNodeType;

typedef struct {
	GfxcAstNodeType type;
	u32 line;
	u32 column;
	u32 next;
	union {
		struct {
			const char *src;
			u32 count;
		} head;
		struct {
			const char *id;
			u32 idLength;
		} texture;
		struct {
			const char *id;
			u32 idLength;
		} region;
		struct {
			const char *id;
			u32 idLength;
		} dataDecl;
		struct {
			const char *id;
			u32 idLength;
		} field;
		struct {
			const char *id;
			u32 idLength;
		} script;
		struct {
			const char *id;
			u32 idLength;
		} label;
		struct {
			f32 offset;
			bool relative;
		} timeLabel;
		struct {
			const char *id;
			u32 idLength;
		} identifier;
		struct {
			i32 value;
		} intLiteral;
		struct {
			bool value;
		} boolLiteral;
		struct {
			f32 value;
		} floatLiteral;
		struct {
			u32 value;
		} hexLiteral;
		struct {
			const char *data;
			u32 dataLength;
		} stringLiteral;
	} data;
} GfxcAstNode;

typedef enum {
	GFXC_TYPE_NONE,

	GFXC_TYPE_R_REGISTER = 1 << 0,
	GFXC_TYPE_RW_REGISTER = GFXC_TYPE_R_REGISTER | (1 << 1),

	GFXC_TYPE_TEXTURE = 1 << 2,
	GFXC_TYPE_REGION = 1 << 3,
	GFXC_TYPE_SCRIPT = 1 << 4,

	GFXC_TYPE_HEX = 1 << 5,
	GFXC_TYPE_STRING = 1 << 6,

	GFXC_TYPE_INT = 1 << 7,
	GFXC_TYPE_BOOL = 1 << 8,
	GFXC_TYPE_FLOAT = GFXC_TYPE_INT | (1 << 9),

	GFXC_TYPE_LABEL = 1 << 10,

	GFXC_TYPE_ENUM_COUNT
} GfxcType;

typedef union {
	struct {
		GfxcType type;
	} shared;
	struct {
		GfxcType type;
		u32 id;
	} texture;
	struct {
		GfxcType type;
		u32 id;
	} region;
	struct {
		GfxcType type;
		u32 id;
	} script;
	struct {
		GfxcType type;
		u32 id;
	} reg;
	struct {
		GfxcType type;
		u32 value;
	} hex;
	struct {
		GfxcType type;
		i32 value;
	} integer;
	struct {
		GfxcType type;
		bool value;
	} boolv;
	struct {
		GfxcType type;
		f32 value;
	} floatv;
	struct {
		GfxcType type;
		u32 to;
	} label;
	struct {
		GfxcType type;
		u32 dataLength;
		const char *data;
	} str;
} GfxcValue;

typedef enum {
	GFXC_TEXTURE_FIELD_PATH,
	GFXC_TEXTURE_FIELD_HAS_DATA,
	GFXC_TEXTURE_FIELD_FORMAT,
	GFXC_TEXTURE_FIELD_WIDTH,
	GFXC_TEXTURE_FIELD_HEIGHT,
	GFXC_TEXTURE_FIELD_ENUM_COUNT
} GfxcTextureField;

typedef enum {
	GFXC_REGION_FIELD_TEXTURE,
	GFXC_REGION_FIELD_X,
	GFXC_REGION_FIELD_Y,
	GFXC_REGION_FIELD_WIDTH,
	GFXC_REGION_FIELD_HEIGHT,
	GFXC_REGION_FIELD_ENUM_COUNT
} GfxcRegionField;

typedef union {
	struct {
		u32 id;
	} texture;
	struct {
		GfxcTextureField field;
	} textureKey;
	struct region {
		u32 id;
	} region;
	struct {
		GfxcRegionField field;
	} regionKey;
	struct {
		u32 id;
	} script;
	struct {
		u32 id;
	} instruction;
	GfxcValue value;
} GfxcAstAnnotation;

typedef enum {
	GFXC_SYMBOL_NONE,
	GFXC_SYMBOL_CONSTANT,
	GFXC_SYMBOL_TEXTURE,
	GFXC_SYMBOL_REGION,
	GFXC_SYMBOL_SCRIPT,
	GFXC_SYMBOL_REG,
	GFXC_SYMBOL_LABEL,
	GFXC_SYMBOL_ENUM_COUNT
} GfxcSymbolType;

typedef struct {
	GfxcSymbolType type;
	u32 idLength;
	const char *id;
} GfxcSymbolShared;

typedef union {
	GfxcSymbolShared shared;
	struct {
		GfxcSymbolShared shared;
		GfxcValue value;
	} constant;
	struct {
		GfxcSymbolShared shared;
		u32 numId;
	} texture;
	struct {
		GfxcSymbolShared shared;
		u32 numId;
	} region;
	struct {
		GfxcSymbolShared shared;
		u32 numId;
	} script;
	struct {
		GfxcSymbolShared shared;
		GfxcType regType;
		i32 idx;
	} reg;
	struct {
		GfxcSymbolShared shared;
		u32 to;
	} label;
} GfxcSymbol;

typedef struct {
	const char *name;
	u32 id;
	u32 argc;
	GfxcType *argt;
} GfxcInstruction;

GfxcAstNode *GFXC_Parse(const char *src);

GfxcAstAnnotation *GFXC_Analyze(const GfxcAstNode *ast);

void GFXC_Error(const char *message, u32 line, u32 column, const char *src);

#endif // GFXC_H
