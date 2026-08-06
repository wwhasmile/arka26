#include <gfxc.h>

#include <stack.h>

#include <string.h>

#define GFXC_ALIGN(x) ((x + 3) & ~3)

typedef struct {
	const GfxcAstNode *ast;
	const GfxcAstAnnotation *annotations;
	u8 *bytecode;
} GeneratorState;

static void Root(GeneratorState *state);

u8 *GFXC_Generate(const GfxcAstNode *ast, const GfxcAstAnnotation *annotations)
{
	GeneratorState state = {
		ast,
		annotations,
		Stack_InitCapacity(sizeof(u8), sizeof(u32) * Stack_Count(ast))
	};

	GfxcBytecodeHeader header = {
		0x00584647,
		0,
		0
	};
	state.bytecode = Stack_PushBulk(state.bytecode, &header, sizeof(header), NULL);

	Root(&state);

	return state.bytecode;
}

static u32 Value(GeneratorState *state, u32 idx, bool *isRegister);

static bool FindString(GeneratorState *state, const char *data, u32 dataLength, u32 *offset);
static void EmitString(GeneratorState *state, const char *data, u32 dataLength);

static void Strings(GeneratorState *state);

static void Textures(GeneratorState *state);
static void Regions(GeneratorState *state);

static void Scripts(GeneratorState *state);

void Root(GeneratorState *state)
{
	Strings(state);

	Textures(state);
	Regions(state);
	Scripts(state);
}

static inline void InternedString(GeneratorState *state, u32 idx);

void Strings(GeneratorState *state)
{
	for (u32 i = 1, j = Stack_Count(state->ast); i < j; ++i) {
		if (state->ast[i].type != GFXC_AST_NODE_STRING_LITERAL)
			continue;
		InternedString(state, i);
	}

	GfxcBytecodeHeader *header = (GfxcBytecodeHeader*)state->bytecode;
	header->start = Stack_Count(state->bytecode);
}

inline void InternedString(GeneratorState *state, u32 idx)
{
	const char *data = state->ast[idx].data.stringLiteral.data;
	u32 dataLength = state->ast[idx].data.stringLiteral.dataLength;
	if (!FindString(state, data, dataLength, NULL))
		EmitString(state, data, dataLength);
}

bool FindString(GeneratorState *state, const char *data, u32 dataLength, u32 *offset)
{
	u8 *bytecode = state->bytecode;

	u32 i = sizeof(GfxcBytecodeHeader);
	u32 count = Stack_Count(bytecode);
	while (i < count) {
		u32 iLength = *(u32*)(bytecode + i);

		if (dataLength != iLength) {
			i += GFXC_ALIGN(iLength) + sizeof(u32);
	 		continue;
		}

		if (strncmp((const char*)bytecode + i + sizeof(u32),
			data, dataLength) == 0) {
			if (offset != NULL)
				*offset = i;
			return true;
		}

		i += GFXC_ALIGN(iLength) + sizeof(u32);
	}

	return false;
}

void EmitString(GeneratorState *state, const char *data, u32 dataLength)
{
	u32 aligned = GFXC_ALIGN(dataLength);

	state->bytecode = Stack_PushBulk(state->bytecode, &dataLength, sizeof(u32), NULL);
	state->bytecode = Stack_PushBulk(state->bytecode, data, dataLength, NULL);

	if (dataLength == aligned)
		return;
	u32 padding = 0;
	state->bytecode = Stack_PushBulk(state->bytecode, &padding, aligned - dataLength, NULL);
}

static void EmitTexture(GeneratorState *state, u32 idx);

void Textures(GeneratorState *state)
{
	u32 idx = 0;
	state->bytecode = Stack_PushBulk(state->bytecode, &idx, sizeof(u32), &idx);

	for (u32 i = 1; i != 0; i = state->ast[i].next) {
		if (state->ast[i].type != GFXC_AST_NODE_TEXTURE)
			continue;
		EmitTexture(state, i);
		++*(u32*)(state->bytecode + idx);
	}
}

void EmitTexture(GeneratorState *state, u32 idx)
{
	const GfxcAstNode *ast = state->ast;

	u32 bytes[ARRAY_LENGTH(GFXC_TEXTURE_ATTRIBUTES)] = { 0 };

	for (u32 i = 0; i < ARRAY_LENGTH(GFXC_TEXTURE_ATTRIBUTES); ++i) {
		bytes[i] = GFXC_TEXTURE_ATTRIBUTES[i].value.shared.bytes;
	}

	for (u32 i = idx + 1; i != 0; i = ast[i].next) {
		u32 attri = state->annotations[i + 1].attributeValue.id;
		bytes[attri] = Value(state, i + 1, NULL);
	}

	state->bytecode = Stack_PushBulk(state->bytecode, bytes, sizeof(bytes), NULL);
}

static void EmitRegion(GeneratorState *state, u32 idx);

void Regions(GeneratorState *state)
{
	u32 idx = 0;
	state->bytecode = Stack_PushBulk(state->bytecode, &idx, sizeof(u32), &idx);

	for (u32 i = 1; i != 0; i = state->ast[i].next) {
		if (state->ast[i].type != GFXC_AST_NODE_REGION)
			continue;
		EmitRegion(state, i);
		++*(u32*)(state->bytecode + idx);
	}
}

void EmitRegion(GeneratorState *state, u32 idx)
{
	const GfxcAstNode *ast = state->ast;

	u32 bytes[ARRAY_LENGTH(GFXC_REGION_ATTRIBUTES)] = { 0 };

	for (u32 i = 0; i < ARRAY_LENGTH(GFXC_REGION_ATTRIBUTES); ++i) {
		bytes[i] = GFXC_REGION_ATTRIBUTES[i].value.shared.bytes;
	}

	for (u32 i = idx + 1; i != 0; i = ast[i].next) {
		u32 attri = state->annotations[i + 1].attributeValue.id;
		bytes[attri] = Value(state, i + 1, NULL);
	}

	state->bytecode = Stack_PushBulk(state->bytecode, bytes, sizeof(bytes), NULL);
}

static void EmitScript(GeneratorState *state, u32 idx);

void Scripts(GeneratorState *state)
{
	u32 idx = 0;
	state->bytecode = Stack_PushBulk(state->bytecode, &idx, sizeof(u32), &idx);

	for (u32 i = 1; i != 0; i = state->ast[i].next) {
		if (state->ast[i].type != GFXC_AST_NODE_SCRIPT)
			continue;
		EmitScript(state, i);
		++*(u32*)(state->bytecode + idx);
	}
}

static void EmitInstruction(GeneratorState *state, u32 idx, f32 time);

void EmitScript(GeneratorState *state, u32 idx)
{
	const GfxcAstNode *ast = state->ast;

	EmitString(state, ast[idx].data.attrSet.id, ast[idx].data.attrSet.idLength);

	u32 start = Stack_Count(state->bytecode);
	u32 sizeIdx = 0;
	state->bytecode = Stack_PushBulk(state->bytecode, &sizeIdx, sizeof(u32), &sizeIdx);

	f32 time = 0.0f;
	for (u32 i = idx + 1; i != 0; i = ast[i].next) {
		if (ast[i].type == GFXC_AST_NODE_TIME_LABEL) {
			if (ast[i].data.timeLabel.relative)
				time += ast[i].data.timeLabel.offset;
			else
				time = ast[i].data.timeLabel.offset;
			continue;
		}
		if (ast[i].type != GFXC_AST_NODE_INSTRUCTION)
			continue;
		EmitInstruction(state, i, time);
	}

	*((u32*)(state->bytecode + sizeIdx)) = Stack_Count(state->bytecode) - start;
}

void EmitInstruction(GeneratorState *state, u32 idx, f32 time)
{
	const GfxcAstAnnotation *annotation = state->annotations + idx;

	state->bytecode = Stack_PushBulk(state->bytecode, &time, sizeof(f32), NULL);

	u16 opcode = annotation->instruction.opcode;
	state->bytecode = Stack_PushBulk(state->bytecode, &opcode, sizeof(u16), NULL);

	u32 maskIdx;
	u16 mask = 0;
	state->bytecode = Stack_PushBulk(state->bytecode, &mask, sizeof(u16), &maskIdx);

	for (u32 i = idx + 1, j = 0; i != 0; i = state->ast[i].next, ++j) {
		bool isRegister;
		u32 bytes = Value(state, i, &isRegister);
		state->bytecode = Stack_PushBulk(state->bytecode, &bytes, sizeof(u32), NULL);

		if (isRegister)
			*((u16*)(state->bytecode + maskIdx)) |= (1 << j);
	}
}

static u32 ResolveLabel(GeneratorState *state, u32 idx);

u32 Value(GeneratorState *state, u32 idx, bool *isRegister)
{
	const GfxcAstAnnotation *annotation = state->annotations + idx;
	u32 bytes = 0;

	if (isRegister != NULL)
		*isRegister = false;

	GfxcValue data = annotation->value.data;
	switch (annotation->value.argt) {
	case GFXC_TYPE_INT:
		if (data.shared.type == GFXC_TYPE_HEX) {
			i32 cvt = (i32)data.hex.value;
			memcpy(&bytes, &cvt, sizeof(u32));
			break;
		}
		if (data.shared.type == GFXC_TYPE_INT_REGISTER) {
			bytes = data.reg.id;
			if (isRegister != NULL)
				*isRegister = true;
			break;
		}
		bytes = data.shared.bytes;
		break;
	case GFXC_TYPE_BOOL:
		if (data.shared.type == GFXC_TYPE_INT_REGISTER) {
			bytes = data.reg.id;
			if (isRegister != NULL)
				*isRegister = true;
			break;
		}
		bytes = data.shared.bytes;
		break;
	case GFXC_TYPE_FLOAT:
		if (data.shared.type == GFXC_TYPE_INT) {
			f32 cvt = (f32)data.intv.value;
			memcpy(&bytes, &cvt, sizeof(u32));
			break;
		}
		if (data.shared.type == GFXC_TYPE_FLOAT_REGISTER) {
			bytes = data.reg.id;
			if (isRegister != NULL)
				*isRegister = true;
			break;
		}
		bytes = data.shared.bytes;
		break;
	case GFXC_TYPE_LABEL:
		bytes = ResolveLabel(state, idx);
		break;
	case GFXC_TYPE_STRING:
		FindString(state, data.string.data, data.string.dataLength, &bytes);
		break;
	default: bytes = data.shared.bytes;
	}

	return bytes;
}

u32 ResolveLabel(GeneratorState *state, u32 idx)
{
	const GfxcAstNode *ast = state->ast;
	const GfxcAstAnnotation *annotations = state->annotations;
	GfxcValue data = annotations[idx].value.data;

	while (ast[idx].type != GFXC_AST_NODE_INSTRUCTION) {
		--idx;
	}

	i32 offset = 0;
	i32 sign = 1;
	if (data.label.to < annotations[idx].instruction.id) {
		sign = -1;
		--idx;
	}

	while (ast[idx].type != GFXC_AST_NODE_INSTRUCTION ||
		annotations[idx].instruction.id != data.label.to) {
		if (ast[idx].type == GFXC_AST_NODE_INSTRUCTION)
			offset += (sizeof(f32) + sizeof(u32)) * sign;
		else if (ast[idx].type != GFXC_AST_NODE_TIME_LABEL && ast[idx].type != GFXC_AST_NODE_LABEL)
			offset += sizeof(u32) * sign;
		idx += sign;
	}

	return (u32)offset;
}
