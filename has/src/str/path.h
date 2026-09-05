#ifndef HAS_STR_PATH_H
#define HAS_STR_PATH_H

#include <defines.h>

u32 Path_Normalize(char *buffer, u32 n, const char *str, u32 strn);

u32 Path_Join(char *buffer, u32 n, const char *a, u32 an, const char *b, u32 bn);

#endif // HAS_STR_PATH_H
