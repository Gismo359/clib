#include <gismo359/array.h>

#include <stdlib.h>

#include <string.h>

i32 Array_DefaultSpaceship(Array * array, void * lhs, void * rhs)
{
    assert(array);
    assert(lhs);
    assert(rhs);

    return memcmp(lhs, rhs, array->stride);
}

bool Array_DefaultEquality(Array * array, void * lhs, void * rhs)
{
    assert(array);
    assert(lhs);
    assert(rhs);

    return Array_DefaultSpaceship(array, lhs, rhs) == 0;
}

void Array_DefaultDestroy(Array * array, void * element)
{
}

Array Array_New(i32 stride)
{
    assert(stride > 0);

    return (Array){
        .buffer = null,
        .count  = 0,
        .room   = 0,
        .stride = stride,
        .alien  = false,
    };
}

byte * Array_GetBuffer(Array * array)
{
    assert(array);

    return array->buffer;
}

i32 Array_GetCount(Array * array)
{
    assert(array);

    return array->count;
}

i32 Array_GetRoom(Array * array)
{
    assert(array);

    return array->room;
}

i32 Array_GetStride(Array * array)
{
    assert(array);

    return array->stride;
}

bool Array_IsAlien(Array * array)
{
    assert(array);

    return array->alien;
}

bool Array_IsEmpty(Array * array)
{
    assert(array);

    return array->count == 0;
}

byte * Array_Get(Array * array, i32 idx)
{
    assert(array);
    assert(idx >= 0);
    assert(idx < array->count);

    return &array->buffer[idx * array->stride];
}

byte * Array_GetFirst(Array * array)
{
    assert(array);

    return Array_Get(array, 0);
}

byte * Array_GetLast(Array * array)
{
    assert(array);

    return Array_Get(array, array->count - 1);
}

Array Array_ViewData(void * data, i32 stride, i32 count)
{
    assert(data != null || count == 0);
    assert(stride > 0);
    assert(count >= 0);

    return (Array){
        .buffer = data,
        .count  = count,
        .room   = count,
        .stride = stride,
        .alien  = true,
    };
}

Array Array_View(Array * array)
{
    assert(array);

    return Array_ViewData(array->buffer, array->stride, array->count);
}

Array Array_ViewSubrange(Array * array, i32 start, i32 stop)
{
    assert(array);
    assert(start <= stop);
    assert(start >= 0);
    assert(stop <= array->count);

    byte * data = array->buffer + start * array->stride;
    i32 stride  = array->stride;
    i32 count   = stop - start;
    return Array_ViewData(data, stride, count);
}

void Array_Reserve(Array * array, i32 room)
{
    assert(array);
    assert(!array->alien);
    assert(room >= 0);

    if (array->room >= room)
    {
        return;
    }

    room--;
    room |= room >> 1;
    room |= room >> 2;
    room |= room >> 4;
    room |= room >> 8;
    room |= room >> 16;
    room++;

    array->buffer = realloc(array->buffer, room * array->stride);
    array->room   = room;
}

void Array_Resize(Array * array, i32 count)
{
    assert(array);
    assert(count >= 0);

    i32 old_size = array->count * array->stride;
    i32 new_size = count * array->stride;
    if (new_size > old_size)
    {
        Array_Reserve(array, count);
        memset(array->buffer + old_size, 0, new_size - old_size);
    }

    array->count = count;
}

void Array_Grow(Array * array, i32 count)
{
    assert(array);
    assert(count >= 0);

    Array_Resize(array, array->count + count);
}

void Array_AppendData(Array * array, void * data, i32 count)
{
    assert(array);
    assert(data != null || count == 0);
    assert(count >= 0);

    Array view = Array_ViewData(data, array->stride, count);
    Array_Append(array, &view);
}

void Array_Append(Array * lhs, Array * rhs)
{
    assert(lhs);
    assert(rhs);

    assert(lhs->stride == rhs->stride);

    // Save size in case lhs == rhs
    i32 lhs_count = Array_GetCount(lhs);
    i32 rhs_count = Array_GetCount(rhs);
    Array_Resize(lhs, lhs_count + rhs_count);

    memcpy(lhs->buffer + lhs_count * lhs->stride, rhs->buffer, rhs_count * rhs->stride);
}

Array Array_Copy(Array * array)
{
    assert(array);

    Array new = Array_New(array->stride);
    Array_Append(&new, array);
    return new;
}

void Array_Swap(Array * lhs, Array * rhs)
{
    assert(lhs);
    assert(rhs);

    Array tmp = *lhs;

    *lhs = *rhs;
    *rhs = tmp;
}

bool Array_EqualsEx(Array * lhs, Array * rhs, EqualityFunction function, void * data)
{
    assert(lhs);
    assert(rhs);
    assert(function);

    assert(lhs->stride == rhs->stride);

    if (lhs == rhs)
    {
        return true;
    }

    if (lhs->count != rhs->count)
    {
        return false;
    }

    // TODO@Daniel: Reuse `Array_DefaultEquality` instead of hardcoding memcmp
    if (function == (EqualityFunction)Array_DefaultEquality)
    {
        return memcmp(lhs->buffer, rhs->buffer, lhs->count * rhs->stride) == 0;
    }

    for (i32 idx = 0; idx < lhs->count; idx++)
    {
        if (!function(data, &lhs->buffer[idx], &rhs->buffer[idx]))
        {
            return false;
        }
    }

    return true;
}

bool Array_Equals(Array * lhs, Array * rhs)
{
    assert(lhs);
    assert(rhs);

    return Array_EqualsEx(lhs, rhs, (EqualityFunction)Array_DefaultEquality, lhs);
}

i32 Array_IndexOfEx(Array * array, void * value, EqualityFunction function, void * data)
{
    assert(array);
    assert(value);
    assert(function);

    if (function == null)
    {
        function = (EqualityFunction)Array_DefaultEquality;
        data     = array;
    }

    for (i32 idx = 0; idx < array->count; idx++)
    {
        void * element = Array_Get(array, idx);
        if (function(data, element, value))
        {
            return idx;
        }
    }

    return -1;
}

i32 Array_IndexOf(Array * array, void * value)
{
    assert(array);
    assert(value);

    return Array_IndexOfEx(array, value, (EqualityFunction)Array_DefaultEquality, array);
}

byte * Array_FindEx(Array * array, void * value, EqualityFunction function, void * data)
{
    assert(array);
    assert(value);
    assert(function);

    i32 idx = Array_IndexOfEx(array, value, function, data);
    if (idx == -1)
    {
        return null;
    }

    return Array_Get(array, idx);
}

byte * Array_Find(Array * array, void * value)
{
    assert(array);
    assert(value);

    return Array_FindEx(array, value, (EqualityFunction)Array_DefaultEquality, array);
}

bool Array_ContainsEx(Array * array, void * value, EqualityFunction function, void * data)
{
    assert(array);
    assert(value);
    assert(function);

    return Array_FindEx(array, value, function, data) != null;
}

bool Array_Contains(Array * array, void * value)
{
    assert(array);
    assert(value);

    return Array_ContainsEx(array, value, (EqualityFunction)Array_DefaultEquality, array);
}

static inline void __memswap(byte * restrict lhs, byte * restrict rhs, i32 size)
{
    /* Use multiple small memcpys with constant size to enable inlining on most
       targets.  */
    enum
    {
        SWAP_GENERIC_SIZE = 32
    };
    byte temp_block[SWAP_GENERIC_SIZE];
    while (size > SWAP_GENERIC_SIZE)
    {
        memcpy(temp_block, lhs, SWAP_GENERIC_SIZE);
        lhs = __mempcpy(lhs, rhs, SWAP_GENERIC_SIZE);
        rhs = __mempcpy(rhs, temp_block, SWAP_GENERIC_SIZE);
        size -= SWAP_GENERIC_SIZE;
    }
    while (size > 0)
    {
        byte temp_byte = lhs[--size];
        lhs[size]      = rhs[size];
        rhs[size]      = temp_byte;
    }
}

static inline void Array_MakeHeap(byte * start, i32 count, i32 stride, SpaceshipFunction function, void * data)
{
    assert(count > 2);

    i32 first_parent    = (count - 3) / 2;
    i32 first_right_kid = (first_parent + 1) * 2;
    for (;; --first_parent, first_right_kid -= 2)
    {
        byte lucifer[stride];
        memcpy(&lucifer, start + stride * first_parent, stride);

        i32 parent    = first_parent;
        i32 right_kid = first_right_kid;

        for (;;)
        {
            i32 kids_cmp = function(data, start + stride * (right_kid - 1), start + stride * right_kid);
            i32 junior = right_kid - (kids_cmp <= 0);

            byte * junior_ref = start + stride * junior;
            i32 devils_cmp    = function(data, &lucifer, junior_ref);
            if (devils_cmp <= 0)
            {
                break;
            }

            if (parent != junior)
            {
                memcpy(start + stride * parent, junior_ref, stride);
            }

            parent    = junior;
            right_kid = (junior + 1) * 2;
            if (right_kid >= count)
            {
                goto write;
            }
        }

        if (parent != first_parent)
        {
        write:
            memcpy(start + stride * parent, &lucifer, stride);
        }

        if (first_parent == 0)
        {
            break;
        }
    }

    for (i32 idx = count - 1;;)
    {
        i32 parent = (idx - 1) / 2;

        byte * idx_ref    = start + stride * idx;
        byte * parent_ref = start + stride * parent;
        if (function(data, parent_ref, idx_ref) != 1)
        {
            break;
        }

        __memswap(idx_ref, parent_ref, stride);

        if (parent == 0)
        {
            break;
        }

        idx = parent;
    }
}

static inline void Array_UnguardedInsertionSort(byte * start, byte * stop, i32 stride, SpaceshipFunction function, void * data)
{
    assert(start <= stop);

    for (byte * current = start; current != stop; current += stride)
    {
        byte placeholder[stride];
        memcpy(placeholder, current, stride);

        for (byte * previous = current;; previous -= stride)
        {
            if (function(data, previous - stride, placeholder) <= 0)
            {
                memcpy(previous, placeholder, stride);
                break;
            }

            memcpy(previous, previous - stride, stride);
        }
    }
}

static inline void Array_SmallSort(byte * start, byte * stop, i32 stride, SpaceshipFunction function, void * data)
{
    assert(start < stop);

    if (stop - start < 3 * stride)
    {
        byte * lhs = start;
        byte * rhs = lhs + stride * (start + stride != stop);

        if (function(data, lhs, rhs) > 0)
        {
            __memswap(lhs, rhs, stride);
        }

        return;
    }

    Array_MakeHeap(start, (stop - start) / stride, stride, function, data);
    Array_UnguardedInsertionSort(start + 2 * stride, stop, stride, function, data);
}

static inline void Array_QuickSort(byte * start, byte * stop, i32 stride, SpaceshipFunction function, void * data)
{
    while ((stop - start) > 256 * stride)
    {
        byte * middle = start + stride * (stop - start) / 2;
        Array_QuickSort(start, middle, stride, function, data);
        stop = middle;
    }

    if (start != stop)
    {
        Array_SmallSort(start, stop, stride, function, data);
    }
}

void Array_SortEx(Array * array, SpaceshipFunction function, void * data)
{
    assert(array);
    assert(function);

    Array_QuickSort(array->buffer, array->buffer + array->stride * array->count, array->stride, function, data);
}

void Array_Sort(Array * array)
{
    assert(array);

    return Array_SortEx(array, (SpaceshipFunction)Array_DefaultSpaceship, array);
}

void Array_DestroyEx(Array * array, DestroyFunction function, void * data)
{
    assert(array);
    assert(!array->alien);
    assert(function);

    if (function == (DestroyFunction)Array_DefaultDestroy)
    {
        goto end;
    }

    for (i32 idx = 0; idx < array->count; idx++)
    {
        void * element = Array_Get(array, idx);
        function(data, element);
    }

end:
    free(array->buffer);

    *array = Array_New(array->stride);
}

void Array_Destroy(Array * array)
{
    assert(array);
    assert(!array->alien);

    return Array_DestroyEx(array, (DestroyFunction)Array_DefaultDestroy, array);
}