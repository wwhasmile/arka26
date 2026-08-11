#ifndef GFXC_H
#define GFXC_H

#include <core/defines.h>

typedef enum {
	GFXC_AST_NODE_NONE,

	GFXC_AST_NODE_HEAD,

	GFXC_AST_NODE_TEXTURE,
	GFXC_AST_NODE_REGION,
	GFXC_AST_NODE_CONSTANT,

	GFXC_AST_NODE_FIELD,

	GFXC_AST_NODE_SCRIPT,
	GFXC_AST_NODE_INSTRUCTION,
	GFXC_AST_NODE_LABEL,
	GFXC_AST_NODE_TIME_LABEL,

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
			u32 attr;
		} texture;
		struct {
			const char *id;
			u32 idLength;
			u32 attr;
		} region;
		struct {
			const char *id;
			u32 idLength;
			u32 attr;
		} attrSet;
		struct {
			const char *id;
			u32 idLength;
		} attr;
		struct {
			const char *id;
			u32 idLength;
			u32 stat;
		} script;
		struct {
			const char *id;
			u32 idLength;
			u32 arg;
		} instruction;
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

	GFXC_TYPE_TEXTURE,
	GFXC_TYPE_REGION,
	GFXC_TYPE_SCRIPT,

	GFXC_TYPE_HEX,
	GFXC_TYPE_INT,
	GFXC_TYPE_BOOL,
	GFXC_TYPE_FLOAT,

	GFXC_TYPE_LABEL,
	GFXC_TYPE_INT_REGISTER,
	GFXC_TYPE_FLOAT_REGISTER,
	GFXC_TYPE_STRING,

	GFXC_TYPE_ENUM_COUNT
} GfxcType;

typedef union {
	struct {
		GfxcType type;
		u32 bytes;
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
		i32 id;
	} reg;
	struct {
		GfxcType type;
		u32 value;
	} hex;
	struct {
		GfxcType type;
		i32 value;
	} intv;
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
	} string;
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
		u16 opcode;
	} instruction;
	struct {
		GfxcType argt;
		GfxcValue data;
	} value;
	struct {
		GfxcType argt;
		GfxcValue data;
		u32 id;
	} attributeValue;
} GfxcAstAnnotation;

typedef struct {
	const char *id;
	u32 idLength;
	GfxcValue value;
} GfxcSymbol;

typedef struct {
	const char *name;
	u32 nameLength;
	u16 argc;
	GfxcType argt[10];
} GfxcInstruction;

typedef struct {
	const char *id;
	u32 idLength;
	bool mandatory;
	GfxcValue value;
} GfxcAttribute;

typedef struct {
	u32 magic;
	u32 version;
	u32 start;
} GfxcBytecodeHeader;

extern const GfxcAttribute GFXC_TEXTURE_ATTRIBUTES[5];
extern const GfxcAttribute GFXC_REGION_ATTRIBUTES[5];
extern const GfxcSymbol GFXC_DEFAULT_SYMBOLS[];
extern const GfxcInstruction GFXC_INSTRUCTIONS[];

GfxcAstNode *GFXC_Parse(const char *src);

GfxcAstAnnotation *GFXC_Analyze(const GfxcAstNode *ast);

u8 *GFXC_Generate(const GfxcAstNode *ast, const GfxcAstAnnotation *annotations);

void GFXC_Dump(const u8 *bytecode, u32 size);

void GFXC_Error(const char *message, u32 line, u32 column, const char *src);

#endif // GFXC_H
