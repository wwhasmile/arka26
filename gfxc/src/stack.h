#ifndef STACK_H
#define STACK_H

#include <defines.h>

#ifndef STACK_DEFAULT_CAPACITY
#define STACK_DEFAULT_CAPACITY 8
#endif // STACK_DEFAULT_CAPACITY

typedef struct Stack {
	void *ptr;
	u32 elemSize;
	u32 capacity;
	u32 count;
} Stack;

Stack Stack_InitCapacity(u32 elemSize, u32 capacity);

inline Stack Stack_Init(u32 elemSize)
{
	return Stack_InitCapacity(elemSize, STACK_DEFAULT_CAPACITY);
}

u32 Stack_Push(Stack *stack, const void *src);

bool Stack_Pop(Stack *stack, void *dest);

void Stack_Reset(Stack *stack, u32 to);

void Stack_Release(Stack *stack);

#endif // STACK_H
