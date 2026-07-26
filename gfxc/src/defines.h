#ifndef DEFINES_H
#define DEFINES_H

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

#define NULL ((void *)0)

typedef _Bool bool;
enum Bool {
	false,
	true,
};

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

struct RangeI8 {
	i8 min;
	i8 max;
};

struct RangeI16 {
	i16 min;
	i16 max;
};

struct RangeI32 {
	i32 min;
	i32 max;
};

struct RangeI64 {
	i64 min;
	i64 max;
};

struct RangeU8 {
	u8 min;
	u8 max;
};

struct RangeU16 {
	u16 min;
	u16 max;
};

struct RangeU32 {
	u32 min;
	u32 max;
};

struct RangeU64 {
	u64 min;
	u64 max;
};

struct RangeF32 {
	f32 min;
	f32 max;
};

struct RangeF64 {
	f64 min;
	f64 max;
};

#define ARRAY_LENGTH(arr) (sizeof((arr)) / (sizeof((arr[0]))))

#endif // DEFINES_H
