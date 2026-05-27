#pragma once

#include <stdint.h>
#include <uchar.h>

#define min(a, b)                                                                                  \
    ({                                                                                             \
        auto _a = a;                                                                               \
        auto _b = b;                                                                               \
        _a < _b ? _a : _b;                                                                         \
    })
#define max(a, b)                                                                                  \
    ({                                                                                             \
        auto _a = a;                                                                               \
        auto _b = b;                                                                               \
        _a > _b ? _a : _b;                                                                         \
    })

static void * const null = 0;

#define Type(...) typeof(__VA_ARGS__)

#define tailed __attribute__((musttail))

#define __name(name, suffix) _##name##_##suffix
#define _name(name, suffix) __name(name, suffix)
#define _tmp(name) _name(name, __LINE__)

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef char8_t c8;
typedef char16_t c16;
typedef char32_t c32;

typedef char8_t byte;
