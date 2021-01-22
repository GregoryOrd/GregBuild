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

void initFileListsAndTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void initTestFiles(TestFileList* testFiles);
void initSourceFiles(SourceFileList* sourceFiles);
void freeObjectFileList(ObjectFileList* list);
void initObjectFileList(ObjectFileList* objectFiles);
void freeFileLists(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void freeTestFileList(TestFileList* list);
void freeSourceFileList(SourceFileList* list);
int removeTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath);

#endif