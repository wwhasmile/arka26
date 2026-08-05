#include "stack.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
	u32 elemSize;
	u32 capacity;
	u32 count;
	u32 _padding0;
} Stack;

void *Stack_InitCapacity(u32 elemSize, u32 capacity)
{
	Stack result = {
		elemSize,
		capacity,
		0,
		0
	};
	Stack *ptr = calloc(sizeof(Stack) + elemSize * capacity, 1);
	*ptr = result;
	return ptr + 1;
}

extern inline void *Stack_Init(u32 elemSize);

void *Stack_PushBulk(void *stack, const void *src, u32 count, u32 *idx)
{
	Stack *ptr = (Stack*)stack - 1;
	if (ptr->count + count > ptr->capacity) {
		u32 capacity = ptr->capacity;
		while (ptr->count + count > capacity)
			capacity *= 2;
		Stack *newPtr = calloc(sizeof(Stack) + ptr->elemSize * capacity, 1);
		memcpy(newPtr, ptr, sizeof(Stack) + ptr->elemSize * ptr->count);
		free(ptr);
		ptr = newPtr;
		ptr->capacity = capacity;
		stack = ptr + 1;
	}
	memcpy((u8*)stack + ptr->elemSize * ptr->count, src, ptr->elemSize * count);
	if (idx != NULL)
		*idx = ptr->count;
	ptr->count += count;
	return stack;
}

extern inline void *Stack_Push(void *stack, const void *src, u32 *idx);

bool Stack_Pop(void *stack, void *dest)
{
	Stack *ptr = (Stack*)stack - 1;
	if (ptr->count == 0)
		return false;
	memcpy(dest, (u8*)stack + ptr->elemSize * --ptr->count, ptr->elemSize);
	return true;
}

void Stack_Reset(void *stack, u32 to)
{
	Stack *ptr = (Stack*)stack - 1;
	if (ptr->count <= to)
		return;
	ptr->count = to;
}

u32 Stack_Count(const void *stack)
{
	Stack *ptr = (Stack*)stack - 1;
	return ptr->count;
}

void Stack_Release(void *stack)
{
	if (stack != NULL)
		free((Stack*)stack - 1);
}
