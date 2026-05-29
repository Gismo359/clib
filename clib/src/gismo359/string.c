#include <gismo359/string.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

String String_New()
{
    return Array_New(1);
}

c8 * String_GetBuffer(String * string)
{
    assert(string);
    assert(string->stride == 1);

    return Array_GetBuffer(string);
}

i32 String_GetCount(String * string)
{
    assert(string);
    assert(string->stride == 1);

    return Array_GetCount(string);
}

i32 String_GetRoom(String * string)
{
    assert(string);
    assert(string->stride == 1);

    return Array_GetRoom(string);
}

bool String_IsAlien(String * string)
{
    assert(string);
    assert(string->stride == 1);

    return Array_IsAlien(string);
}

bool String_IsEmpty(String * string)
{
    assert(string);
    assert(string->stride == 1);

    return Array_IsEmpty(string);
}

c8 * String_Get(String * array, i32 idx)
{
    assert(array);
    assert(idx >= 0);
    assert(idx < array->count);

    return Array_GetAs(c8, array, idx);
}

c8 * String_GetFirst(String * array)
{
    assert(array);

    return Array_GetFirstAs(c8, array);
}

c8 * String_GetLast(String * array)
{
    assert(array);

    return Array_GetLastAs(c8, array);
}

String String_ViewLiteral(c8 * buffer)
{
    return Array_ViewData(buffer, 1, buffer == null ? 0 : strlen((char *)buffer));
}

String String_ViewData(c8 * data, i32 count)
{
    return Array_ViewData(data, 1, count);
}

String String_View(String * string)
{
    assert(string);
    assert(string->stride == 1);

    return Array_View(string);
}

void String_Reserve(String * string, i32 room)
{
    assert(string);
    assert(string->stride == 1);

    Array_Reserve(string, room);
}

void String_Resize(String * string, i32 count)
{
    assert(string);
    assert(string->stride == 1);

    Array_Resize(string, count);
}

void String_Grow(String * string, i32 count)
{
    assert(string);                     
    assert(string->stride == 1);

    Array_Resize(string, string->count + count);
}

i32 String_AppendFormatExV(String * string, i32 size_hint, c8 * format, va_list args)
{
    assert(string);
    assert(size_hint > 0);
    assert(format);

    i32 true_size;
    String tmp = String_ViewStatic((c8[256]){});

    if (size_hint >= 256)
    {
        tmp = String_New();
        String_Resize(&tmp, size_hint);
    }

    c8 * begin   = String_GetBuffer(&tmp);
    i32 capacity = String_GetRoom(&tmp);

    va_list copy;
    va_copy(copy, args);
    true_size = vsnprintf((char *)begin, capacity, (char *)format, args);
    va_end(copy);

    if (true_size < capacity)
    {
        String_Resize(&tmp, true_size);
        String_Append(string, &tmp);
        goto end;
    }

    i32 current_size = String_GetCount(string);
    String_Resize(string, current_size + true_size + 1);

    c8 * end = Array_GetAs(c8, string, current_size);
    vsnprintf((char *)begin, true_size + 1, (char *)format, args);

    String_Resize(string, current_size + true_size);

end:
    if (size_hint >= 256)
    {
        String_Destroy(&tmp);
    }

    return true_size;
}

i32 String_AppendFormatV(String * string, c8 * format, va_list args)
{
    assert(string);
    assert(format);

    return String_AppendFormatExV(string, 255, format, args);
}

i32 String_AppendFormatEx(String * string, i32 size_hint, c8 * format, ...)
{
    assert(string);
    assert(size_hint > 0);
    assert(format);

    va_list args;
    va_start(args, format);
    i32 size = String_AppendFormatExV(string, size_hint, format, args);
    va_end(args);

    return size;
}

i32 String_AppendFormat(String * string, c8 * format, ...)
{
    assert(string);
    assert(format);

    va_list args;
    va_start(args, format);
    i32 size = String_AppendFormatV(string, format, args);
    va_end(args);

    return size;
}

void String_AppendLiteral(String * string, c8 * buffer)
{
    assert(string);
    assert(string->stride == 1);

    String view = String_ViewLiteral(buffer);
    String_Append(string, &view);
}

void String_AppendData(String * string, void * data, i32 count)
{
    assert(string);
    assert(string->stride == 1);

    Array_AppendData(string, data, count);
}

void String_Append(String * lhs, String * rhs)
{
    assert(lhs);
    assert(rhs);
    assert(lhs->stride == 1);
    assert(rhs->stride == 1);

    Array_Append(lhs, rhs);
}

String String_FromFormatExV(i32 size_hint, c8 * format, va_list args)
{
    assert(size_hint > 0);
    assert(format);

    String string = String_New();
    String_AppendFormatExV(&string, size_hint, format, args);
    return string;
}

String String_FromFormatV(c8 * format, va_list args)
{
    assert(format);

    String string = String_New();
    String_AppendFormatV(&string, format, args);
    return string;
}

String String_FromFormatEx(i32 size_hint, c8 * format, ...)
{
    assert(size_hint > 0);
    assert(format);

    va_list args;
    va_start(args, format);
    String string = String_FromFormatExV(size_hint, format, args);
    va_end(args);

    return string;
}

String String_FromFormat(c8 * format, ...)
{
    assert(format);

    va_list args;
    va_start(args, format);
    String string = String_FromFormatV(format, args);
    va_end(args);

    return string;
}

String String_Copy(String * string)
{
    assert(string);
    assert(string->stride == 1);

    return Array_Copy(string);
}

String String_CopyLiteral(c8 * buffer)
{
    String new = String_New();
    String_AppendLiteral(&new, buffer);
    return new;
}

void String_Swap(String * lhs, String * rhs)
{
    assert(lhs);
    assert(rhs);
    assert(lhs->stride == 1);
    assert(rhs->stride == 1);

    return Array_Swap(lhs, rhs);
}

bool String_Equals(String * lhs, String * rhs)
{
    assert(lhs);
    assert(rhs);
    assert(lhs->stride == 1);
    assert(rhs->stride == 1);

    return Array_EqualsEx(lhs, rhs, null, null);
}

c8 * String_Find(String * string, c8 value)
{
    assert(string);
    assert(string->stride == 1);

    return Array_FindEx(string, &value, null, null);
}

bool String_Contains(String * string, c8 value)
{
    assert(string);
    assert(string->stride == 1);

    return String_Find(string, value) != null;
}

void String_Destroy(String * string)
{
    assert(string);
    assert(string->stride == 1);

    Array_DestroyEx(string, (DestroyFunction)Array_DefaultDestroy, string);
}