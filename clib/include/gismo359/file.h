#pragma once

#include <gismo359/string.h>

#include <stdio.h>

typedef FILE * File;

File File_Open(String * path);

void File_ReadInto(File * file, String * destination, i64 size);
String File_Read(File * file, i64 size);

void File_ReadRestInto(File * file, String * destination);
String File_ReadRest(File * file);

void File_ReadAllInto(File * file, String * destination);
String File_ReadAll(File * file);

void File_Move(File * file, i64 offset);
void File_MoveTo(File * file, i64 position);
void File_MoveToStart(File * file);
void File_MoveToEnd(File * file);

i64 File_GetPosition(File * file);
i64 File_GetSize(File * file);

void File_Close(File * file);