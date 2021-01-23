#ifndef FILE_STRUCTURE_DEFS_H
#define FILE_STRUCTURE_DEFS_H

#define WINDOWS_MAX_PATH_LENGTH 260

#include <stdbool.h>

typedef struct ObjectFile
{
    char* name;
    bool isFromSource;
} ObjectFile;

typedef struct ObjectFileList
{
    int size;
    ObjectFile* files;
} ObjectFileList;

typedef struct SourceFile
{
    char* name;
} SourceFile;

typedef struct SourceFileList
{
    int size;
    SourceFile* files;
} SourceFileList;

typedef struct TestCase
{
    char* testName;
} TestCase;

typedef struct TestFile
{
    char* name;
    int numTestCases;
    TestCase* cases;
} TestFile;

typedef struct TestFileList
{
    int size;
    int totalNumTestCases;
    TestFile* files;
} TestFileList;

#endif