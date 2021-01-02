#ifndef FILE_AND_TEST_CASE_GATHERER_H
#define FILE_AND_TEST_CASE_GATHERER_H

#include "SourceFileStructureDefs.h"
#include "TestStructureDefs.h"

#include <dirent.h>
#include <stdbool.h>

void initTestFiles(TestFileList* testFiles);
void initSourceFiles(SourceFileList* sourceFiles);
void loadTestsAndSourceFiles(TestFileList* testFiles, SourceFileList* sourceFiles, char* basePath);
void addToListOrEnterSubDirectoryForRecursion(TestFileList* testFiles, SourceFileList* sourceFiles, char* basePath, struct dirent *fileOrSubDirectory, char* fileOrSubDirectoryFullPath);
void copyFileOrSubDirectoryNameIntoPath(char* path, char* basePath, char* fileOrSubDirectoryName);
void addTestFileToList(TestFileList* testFileList, const char* pathToTestFile);
void addTestCasesToList(TestFileList* testFileList, const char* pathToTestFile);
void addSourceFileToList(SourceFileList* list, const char* pathToSourceFile);
void addSingleTestCaseToList(TestFileList* testFileList, const char* pathToTestFile, char* buffer);
void freeTestFileList(TestFileList* list);
void freeSourceFileList(SourceFileList* list);
bool isVisibleDirectory(struct dirent *fileOrSubDirectory);
void trimTestName(char* testName);

#endif