#pragma once

#include <gismo359/common.h>

#include <assert.h>

#define Array(type) Array

#define Array_ViewStatic(...)                                                                      \
    Array_ViewData(__VA_ARGS__, sizeof(__VA_ARGS__[0]), sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__[0]))
#define Array_AppendStatic(array, ...)                                                             \
    Array_AppendData(array, __VA_ARGS__, sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__[0]))

#define Array_AppendStatic(array, ...)                                                             \
    Array_AppendData(array, __VA_ARGS__, sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__[0]))

#define Array_ViewSingle(...) Array_ViewStatic((typeof(__VA_ARGS__)[1]){__VA_ARGS__})
#define Array_AppendSingle(array, ...)                                                             \
    Array_AppendStatic(array, (typeof(__VA_ARGS__)[1]){__VA_ARGS__})

#define Array_GetAs(type, array, idx)                                                              \
    ({                                                                                             \
        Array(type) * _tmp(get) = array;                                                           \
        assert(_tmp(get));                                                                         \
        assert(sizeof(type) == _tmp(get)->stride);                                                 \
        (typeof(type) *)Array_Get(_tmp(get), idx);                                                 \
    })

#define Array_GetFirstAs(type, array)                                                              \
    ({                                                                                             \
        Array(type) * _tmp(get_first) = array;                                                     \
        assert(_tmp(get_first));                                                                   \
        assert(sizeof(type) == _tmp(get_first)->stride);                                           \
        (typeof(type) *)Array_GetFirst(_tmp(get_first));                                           \
    })

#define Array_GetLastAs(type, array)                                                               \
    ({                                                                                             \
        Array(type) * _tmp(get_last) = array;                                                      \
        assert(_tmp(get_last));                                                                    \
        assert(sizeof(type) == _tmp(get_last)->stride);                                            \
        (typeof(type) *)Array_GetLast(_tmp(get_last));                                             \
    })

#define Array_FindAs(type, array, value, function, data)                                           \
    ({                                                                                             \
        Array(type) * _tmp(find) = array;                                                          \
        assert(_tmp(find));                                                                        \
        assert(sizeof(type) == _tmp(find)->stride);                                                \
        (typeof(type) *)Array_Find(_tmp(find), value, function, data);                             \
    })

#define Array_Iter(array, idx_name, ref_name) Array_IterAs(byte, array, idx_name, ref_name)
#define Array_IterAs(type, array, idx_name, ref_name)                                                 \
    for (Array(type) * _tmp(iter) = array; _tmp(iter) != null; _tmp(iter) = null)                     \
        for (typeof(type) * ref_name = (type *)_tmp(iter)->buffer; ref_name != null; ref_name = null) \
            for (i32 idx_name = 0;                                                                    \
                 (idx_name < Array_GetCount(_tmp(iter)) &&                                            \
                  (ref_name = Array_GetAs(type, _tmp(iter), idx_name), true));                        \
                 idx_name++)

#define Array_IterR(array, idx_name, ref_name) Array_IterAsR(byte, array, idx_name, ref_name)
#define Array_IterAsR(type, array, idx_name, ref_name)                                                \
    for (Array(type) * _tmp(iter) = array; _tmp(iter) != null; _tmp(iter) = null)                     \
        for (typeof(type) * ref_name = (type *)_tmp(iter)->buffer; ref_name != null; ref_name = null) \
            for (i32 idx_name = Array_GetCount(_tmp(iter)) - 1;                                       \
                 (idx_name >= 0 && (ref_name = Array_GetAs(type, _tmp(iter), idx_name), true));       \
                 idx_name++)

#define Array_NewOf(type) Array_New(sizeof(type))
#define Array_ViewOf(type, ...) Array_ViewStatic((type){__VA_ARGS__})
#define Array_CopyOf(type, ...)                                                                    \
    ({                                                                                             \
        Array(type) _tmp(array) = Array_ViewOf(type, __VA_ARGS__);                                 \
        Array_Copy(&_tmp(array));                                                                  \
    })

typedef struct
{
    byte * buffer;
    i32 count;
    i32 room;
    i32 stride;
    bool alien;
} Array;

typedef i32 (*SpaceshipFunction)(void * data, void * lhs, void * rhs);
typedef bool (*EqualityFunction)(void * data, void * lhs, void * rhs);
typedef void (*DestroyFunction)(void * data, void * element);

i32 Array_DefaultSpaceship(Array * array, void * lhs, void * rhs);
bool Array_DefaultEquality(Array * array, void * lhs, void * rhs);
void Array_DefaultDestroy(Array * array, void * element);

Array Array_New(i32 stride);

byte * Array_GetBuffer(Array * array);
i32 Array_GetCount(Array * array);
i32 Array_GetRoom(Array * array);
i32 Array_GetStride(Array * array);
bool Array_IsAlien(Array * array);

bool Array_IsEmpty(Array * array);

byte * Array_Get(Array * array, i32 idx);
byte * Array_GetFirst(Array * array);
byte * Array_GetLast(Array * array);

Array Array_ViewData(void * data, i32 stride, i32 count);
Array Array_View(Array * array);
Array Array_ViewSubrange(Array * array, i32 start, i32 stop);

void Array_Reserve(Array * array, i32 room);
void Array_Resize(Array * array, i32 count);
void Array_Grow(Array * array, i32 count);

void Array_AppendData(Array * array, void * data, i32 count);
void Array_Append(Array * lhs, Array * rhs);

Array Array_Copy(Array * array);

void Array_Swap(Array * lhs, Array * rhs);

bool Array_EqualsEx(Array * lhs, Array * rhs, EqualityFunction function, void * data);
bool Array_Equals(Array * lhs, Array * rhs);

i32 Array_IndexOfEx(Array * array, void * value, EqualityFunction function, void * data);
i32 Array_IndexOf(Array * array, void * value);

byte * Array_FindEx(Array * array, void * value, EqualityFunction function, void * data);
byte * Array_Find(Array * array, void * value);

bool Array_ContainsEx(Array * array, void * value, EqualityFunction function, void * data);
bool Array_Contains(Array * array, void * value);

void Array_SortEx(Array * array, SpaceshipFunction function, void * data);
void Array_Sort(Array * array);

void Array_DestroyEx(Array * array, DestroyFunction function, void * data);
void Array_Destroy(Array * array);