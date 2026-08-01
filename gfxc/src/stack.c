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
	Stack *ptr = malloc(sizeof(Stack) + elemSize * capacity);
	*ptr = result;
	return ptr + 1;
}

extern inline void *Stack_Init(u32 elemSize);

void *Stack_Push(void *stack, const void *src, u32 *idx)
{
	Stack *ptr = (Stack*)stack - 1;
	if (ptr->count == ptr->capacity) {
		u32 capacity = ptr->capacity * 2;
		Stack *newPtr = malloc(sizeof(Stack) + ptr->elemSize * capacity);
		memcpy(newPtr, ptr, sizeof(Stack) + ptr->elemSize * capacity / 2);
		free(ptr);
		ptr = newPtr;
		ptr->capacity = capacity;
	}
	memcpy((u8*)stack + ptr->elemSize * ptr->count, src, ptr->elemSize);
	if (idx != NULL)
		*idx = ptr->count++;
	return ptr + 1;
}

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

u32 Stack_Count(void *stack)
{
	Stack *ptr = (Stack*)stack - 1;
	return ptr->count;
}

void Stack_Release(void *stack)
{
	if (stack != NULL)
		free((Stack*)stack - 1);
}
