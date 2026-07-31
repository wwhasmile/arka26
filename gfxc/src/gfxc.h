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
		} regionRegister;
		struct {
			const char *id;
			u32 idLength;
		} intRegister;
		struct {
			const char *id;
			u32 idLength;
		} floatRegister;
		struct {
			const char *id;
			u32 idLength;
		} identifier;
		struct {
			i32 value;
		} intLiteral;
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
	GFXC_TYPE_FLOAT = 1 << 8,

	GFXC_TYPE_LABEL = 1 << 9,

	GFXC_TYPE_NUMBER = GFXC_TYPE_INT | GFXC_TYPE_FLOAT,

	GFXC_TYPE_ENUM_COUNT
} GfxcType;

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
		GfxcTextureField field;

	} textureKey;
	struct {
		GfxcType type;
	} textureValue;
	struct {
		GfxcRegionField field;
	} regionKey;
	struct {
		GfxcType type;
		union {
			struct {
				u32 id;
			} texture;
		} data;
	} regionValue;
	struct {
		GfxcType type;
		union {
			struct {
				u32 id;
			} region;
			struct {
				u32 id;
			} script;
			struct {
				u32 to;
			} label;
		} data;
	} operand;
} GfxcAstNodeSemanticData;

typedef struct {
	const char *name;
	i32 id;
	GfxcType type;
} GfxcRegister;

typedef struct {
	const char *name;
	u32 id;
	u32 argc;
	GfxcType *argt;
} GfxcInstruction;

GfxcAstNode *GFXC_Parse(const char *src);

GfxcAstNodeSemanticData *GFXC_AnalyzeAST(const GfxcAstNode *ast);

void GFXC_Error(const char *message, u32 line, u32 column, const char *src);

extern const GfxcRegister GFXC_REGISTERS[];
extern const GfxcInstruction GFXC_INSTRUCTIONS[];

#endif // GFXC_H
