#ifndef STACK_H
#define STACK_H

#include <core/defines.h>

#ifndef STACK_DEFAULT_CAPACITY
#define STACK_DEFAULT_CAPACITY 8
#endif // STACK_DEFAULT_CAPACITY

void *Stack_InitCapacity(u32 elemSize, u32 capacity);

inline void *Stack_Init(u32 elemSize)
{
	return Stack_InitCapacity(elemSize, STACK_DEFAULT_CAPACITY);
}

void *Stack_PushBulk(void *stack, const void *src, u32 count, u32 *idx);

inline void *Stack_Push(void *stack, const void *src, u32 *idx)
{
	return Stack_PushBulk(stack, src, 1, idx);
}

bool Stack_Pop(void *stack, void *dest);

void Stack_Reset(void *stack, u32 to);

u32 Stack_Count(const void *stack);

void Stack_Release(void *stack);

#endif // STACK_H
