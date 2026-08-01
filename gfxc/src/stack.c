#include "stack.h"

#include <stdlib.h>
#include <string.h>

#define STACK_DEFAULT_CAPACITY 8

Stack Stack_InitCapacity(u32 elemSize, u32 capacity)
{
	Stack result = {
		NULL,
		elemSize,
		capacity,
		0
	};
	return result;
}

extern inline Stack Stack_Init(u32 elemSize);

u32 Stack_Push(Stack *stack, const void *src)
{
	if (stack->ptr == NULL) {
		stack->count = 0;
		stack->ptr = malloc(stack->elemSize * stack->capacity);
	} else if (stack->count == stack->capacity) {
		u32 capacity = stack->capacity * 2;
		void *ptr = malloc(stack->elemSize * capacity);
		memcpy(ptr, stack->ptr, stack->elemSize * stack->capacity);
		free(stack->ptr);
		stack->ptr = ptr;
		stack->capacity = capacity;
	}
	memcpy((u8*)stack->ptr + stack->elemSize * stack->count, src, stack->elemSize);
	return stack->count++;
}

bool Stack_Pop(Stack *stack, void *dest)
{
	if (stack->ptr == NULL || stack->count == 0)
		return false;
	memcpy(dest, (u8*)stack->ptr + stack->elemSize * --stack->count, stack->elemSize);
	return true;
}

void Stack_Reset(Stack *stack, u32 to)
{
	if (stack->count <= to)
		return;
	stack->count = to;
}

void Stack_Release(Stack *stack)
{
	if (stack->ptr != NULL)
		free(stack->ptr);
}
