#ifndef FILE_STRUCTURE_DEFS_H
#define FILE_STRUCTURE_DEFS_H

#define WINDOWS_MAX_PATH_LENGTH 260

#include <stdbool.h>

#define TEST_FILE_LIST_TYPE 0
#define SRC_FILE_LIST_TYPE 1

typedef struct ObjectFile
{
    char name[WINDOWS_MAX_PATH_LENGTH];
    bool isFromSource;
} ObjectFile;

typedef struct ObjectFileList
{
    int size;
    ObjectFile* files;
} ObjectFileList;

typedef struct SourceFile
{
    char name[WINDOWS_MAX_PATH_LENGTH];
} SourceFile;

typedef struct SourceFileList
{
    int size;
    SourceFile* files;
} SourceFileList;

typedef struct TestCase
{
    char testName[WINDOWS_MAX_PATH_LENGTH];
} TestCase;

typedef struct TestFile
{
    char name[WINDOWS_MAX_PATH_LENGTH];
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