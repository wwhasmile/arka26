#ifndef AST_H
#define AST_H

#include <defines.h>

typedef enum {
	AST_NODE_NONE,

	AST_NODE_HEAD,

	AST_NODE_TEXTURE,
	AST_NODE_REGION,
	AST_NODE_CONSTANT,

	AST_NODE_FIELD,

	AST_NODE_SCRIPT,
	AST_NODE_LABEL,
	AST_NODE_TIME_LABEL,
	AST_NODE_INSTRUCTION,
	AST_NODE_REGION_REGISTER,
	AST_NODE_INT_REGISTER,
	AST_NODE_FLOAT_REGISTER,

	AST_NODE_IDENTIFIER,
	AST_NODE_INT_LITERAL,
	AST_NODE_FLOAT_LITERAL,
	AST_NODE_HEX_LITERAL,
	AST_NODE_STRING_LITERAL,

	AST_NODE_ENUM_COUNT
} AstNodeType;

typedef struct {
	AstNodeType type;
	u32 line;
	u32 column;
	u32 next;
	union {
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
} AstNode;

#endif // AST_H
