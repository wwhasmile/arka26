#ifndef AST_H
#define AST_H

#include <defines.h>

typedef enum {
	AST_NODE_TYPE_NONE,

	AST_NODE_TYPE_HEAD,

	AST_NODE_TYPE_TEXTURE,
	AST_NODE_TYPE_REGION,
	AST_NODE_TYPE_CONSTANT,

	AST_NODE_TYPE_FIELD,

	AST_NODE_TYPE_SCRIPT,
	AST_NODE_TYPE_LABEL,
	AST_NODE_TYPE_TIME_LABEL,
	AST_NODE_TYPE_INSTRUCTION,
	AST_NODE_TYPE_REGION_REGISTER,
	AST_NODE_TYPE_INT_REGISTER,
	AST_NODE_TYPE_FLOAT_REGISTER,

	AST_NODE_TYPE_IDENTIFIER,
	AST_NODE_TYPE_INT_LITERAL,
	AST_NODE_TYPE_FLOAT_LITERAL,
	AST_NODE_TYPE_HEX_LITERAL,
	AST_NODE_TYPE_STRING_LITERAL,

	AST_NODE_TYPE_ENUM_COUNT
} AstNodeType;

typedef struct {
	AstNodeType type;
	u32 line;
	u32 column;
	u32 next;
	union {
		struct {
			u32 count;
		} head;
		struct {
			u32 fields;
		} texture;
		struct {
			u32 fields;
		} region;
		struct {
			u32 value;
		} constant;
		struct {
			const char *id;
			u32 idLength;
			u32 value;
		} field;
		struct {
			const char *id;
			u32 idLength;
			u32 statements;
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
} AstNode;

#endif // AST_H
