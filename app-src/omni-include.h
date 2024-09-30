#pragma once
#include <stdint.h>
#include <stddef.h>
//#include <uchar.h>

//This header is meant to be included in more or less every file in the program.
//inspired by Nullprogram's 2023 C style guide https://nullprogram.com/blog/2023/10/08/

typedef uint8_t   u8;
//typedef char16_t  c16;
typedef int32_t   b32;
typedef int32_t   i32;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef float     f32;
typedef double    f64;
typedef uintptr_t uptr;
typedef char      byte;
typedef ptrdiff_t size;
typedef size_t    usize;


//fat pointer style of string. utf-8 here
#define s8(s) (s8){(u8 *)s, lengthof(s)}
typedef struct {
    u8  *data;
    size len;
} s8;
//and utf-16 here
//#define s16(s) (s16){u##s, lengthof(u##s)}
//typedef struct {
//    c16 *data;
//    size len;
//} s16;

