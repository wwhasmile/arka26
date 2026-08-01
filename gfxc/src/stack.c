#include "stack.h"

typedef struct Stack {
	u32 element;
	u32 capacity;
	u32 count;
	u32 _padding;
} Stack;
