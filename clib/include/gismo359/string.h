#pragma once

#include <gismo359/array.h>

#include <inttypes.h>
#include <stdarg.h>

#define String_ViewStatic(...)                                                                     \
    String_ViewData(__VA_ARGS__, sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__[0]))

typedef Array(c8) String;

String String_New();

c8 * String_GetBuffer(String * string);
i32 String_GetCount(String * string);
i32 String_GetRoom(String * string);
bool String_IsAlien(String * string);

bool String_IsEmpty(String * string);

String String_ViewLiteral(c8 * buffer);
String String_ViewData(c8 * data, i32 count);
String String_View(String * string);

void String_Reserve(String * string, i32 room);
void String_Resize(String * string, i32 count);

i32 String_AppendFormatExV(String * string, i32 size_hint, c8 * format, va_list args);
i32 String_AppendFormatV(String * string, c8 * format, va_list args);

i32 String_AppendFormatEx(String * string, i32 size_hint, c8 * format, ...);
i32 String_AppendFormat(String * string, c8 * format, ...);

void String_AppendLiteral(String * string, c8 * buffer);
void String_AppendData(String * string, void * data, i32 count);
void String_Append(String * lhs, String * rhs);

String String_FromFormatExV(i32 size_hint, c8 * format, va_list args);
String String_FromFormatV(c8 * format, va_list args);

String String_FromFormatEx(i32 size_hint, c8 * format, ...);
String String_FromFormat(c8 * format, ...);

String String_CopyLiteral(c8 * buffer);
String String_Copy(String * string);

void String_Swap(String * lhs, String * rhs);

bool String_Equals(String * lhs, String * rhs);
c8 * String_Find(String * string, c8 value);
bool String_Contains(String * string, c8 value);

void String_Destroy(String * string);