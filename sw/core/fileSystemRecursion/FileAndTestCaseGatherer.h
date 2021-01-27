#ifndef FILE_AND_TEST_CASE_GATHERER_H
#define FILE_AND_TEST_CASE_GATHERER_H

#include "../common/FileStructureDefs.h"

#include <dirent.h>
#include <stdbool.h>

int loadTestsAndSourceFiles(TestFileList *testFiles,
                            SourceFileList *sourceFiles,
                            ObjectFileList *tempObjectFiles,
                            int previousStepFailed, const char *basePath);
void addToListOrEnterSubDirectoryForRecursion(
    TestFileList *testFiles, SourceFileList *sourceFiles,
    ObjectFileList *tempObjectFiles, int previousStepFailed,
    const char *basePath, const struct dirent *fileOrSubDirectory,
    const char *fileOrSubDirectoryFullPath);
void copyFileOrSubDirectoryNameIntoPath(char *path, const char *basePath,
                                        const char *fileOrSubDirectoryName);
void addTestFileToList(TestFileList *testFileList, const char *pathToTestFile);
void addTestCasesToList(TestFileList *testFileList, const char *pathToTestFile);
void addSourceFileToList(SourceFileList *list, const char *pathToSourceFile);
void addSingleTestCaseToList(TestFileList *testFileList,
                             const char *pathToTestFile, char *buffer);
void trimTestName(char *testName);

#endif