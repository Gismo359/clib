#include <gismo359/file.h>

File File_Open(String * path)
{
    String tmp = String_Copy(path);
    Array_AppendSingle(&tmp, u8'\0');

    c8 * buffer = String_GetBuffer(&tmp);
    File file   = fopen((char *)buffer, "rb+");

    String_Destroy(&tmp);

    return file;
}

void File_ReadInto(File * file, String * destination, i64 size)
{
    assert(file);
    assert(destination);
    assert(size >= 0);

    i32 count = String_GetCount(destination);

    String_Grow(destination, size);

    c8 * buffer = String_Get(destination, count);

    fread(buffer, 1, size, *file);
}

String File_Read(File * file, i64 size)
{
    assert(file);

    String string = String_New();
    File_ReadInto(file, &string, size);
    return string;
}

void File_ReadRestInto(File * file, String * destination)
{
    assert(file);
    assert(destination);

    i64 position = File_GetPosition(file);
    i64 size     = File_GetSize(file);

    File_ReadInto(file, destination, size - position);
    File_MoveTo(file, position);
}

String File_ReadRest(File * file)
{
    assert(file);

    String string = String_New();
    File_ReadRestInto(file, &string);
    return string;
}

void File_ReadAllInto(File * file, String * destination)
{
    assert(file);
    assert(destination);

    i64 position = File_GetPosition(file);
    i64 size     = File_GetSize(file);

    File_MoveToStart(file);
    File_ReadInto(file, destination, size);
    File_MoveTo(file, position);
}

String File_ReadAll(File * file)
{
    assert(file);

    String string = String_New();
    File_ReadAllInto(file, &string);
    return string;
}

void File_Move(File * file, i64 offset)
{
    assert(file);
    assert(offset >= 0);

    fseek(*file, offset, SEEK_CUR);
}

void File_MoveTo(File * file, i64 position)
{
    assert(file);
    assert(position >= 0);

    fseek(*file, position, SEEK_SET);
}

void File_MoveToStart(File * file)
{
    assert(file);

    rewind(*file);
}

void File_MoveToEnd(File * file)
{
    assert(file);

    fseek(*file, 0, SEEK_END);
}

i64 File_GetPosition(File * file)
{
    assert(file);

    return ftell(*file);
}

i64 File_GetSize(File * file)
{
    assert(file);

    i64 position = File_GetPosition(file);

    File_MoveToEnd(file);
    i64 size = File_GetPosition(file);
    File_MoveTo(file, position);

    return size;
}

void File_Close(File * file)
{
    assert(file);

    fclose(*file);
}