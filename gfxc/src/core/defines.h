#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <stdbool.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#define NULL ((void *)0)

#define I8_MIN -128
#define I8_MAX 127
#define I16_MIN -32768
#define I16_MAX 32767
#define I32_MIN -2147483648
#define I32_MAX 2147483647
#define I64_MIN -9223372036854775808
#define I64_MAX 9223372036854775807

#define U8_MIN 0
#define U8_MAX 255
#define U16_MIN 0
#define U16_MAX 65535
#define U32_MIN 0
#define U32_MAX 4294967295
#define U64_MIN 0
#define U64_MAX 18446744073709551615

typedef struct {
	i8 min;
	i8 max;
} RangeI8;

typedef struct {
	i16 min;
	i16 max;
} RangeI16;

typedef struct {
	i32 min;
	i32 max;
} RangeI32;

typedef struct {
	i64 min;
	i64 max;
} RangeI64;

typedef struct RangeU8 {
	u8 min;
	u8 max;
} RangeU8;

typedef struct {
	u16 min;
	u16 max;
} RangeU16;

typedef struct {
	u32 min;
	u32 max;
} RangeU32;

typedef struct {
	u64 min;
	u64 max;
} RangeU64;

typedef struct {
	f32 min;
	f32 max;
} RangeF32;

typedef struct {
	f64 min;
	f64 max;
} RangeF64;

#define ARRAY_LENGTH(arr) (sizeof((arr)) / (sizeof((arr[0]))))

#endif // DEFINES_H
