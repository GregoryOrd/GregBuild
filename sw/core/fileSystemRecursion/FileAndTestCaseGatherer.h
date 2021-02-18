#ifndef FILE_AND_TEST_CASE_GATHERER_H
#define FILE_AND_TEST_CASE_GATHERER_H

#include "../common/FileStructureDefs.h"
#include "../../external/GregCToolkit/sw/ExternalProgramExecution/ArgListDefs.h"

#include <dirent.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif  

int loadTestsAndSourceFiles(TestFileList *testFiles,
                            SourceFileList *sourceFiles,
                            ObjectFileList *tempObjectFiles,
                            int errorOnPreviousStep, const char *basePath);
void addToListOrEnterSubDirectoryForRecursion(
    TestFileList *testFiles, SourceFileList *sourceFiles,
    ObjectFileList *tempObjectFiles, int errorOnPreviousStep,
    const char *basePath, const struct dirent *fileOrSubDirectory,
    const char *fileOrSubDirectoryFullPath);
void copyFileOrSubDirectoryNameIntoPath(char *path, const char *basePath,
                                        const char *fileOrSubDirectoryName);
void addTestFileToList(TestFileList *testFileList, const char *pathToTestFile);
void addSourceFileToList(SourceFileList *list, const char *pathToSourceFile);
void checkForMainFunction(const char* pathToSourceFile);
int markMainFunctionLine(ArgList* argList);
void addTestCasesToList(TestFileList* testFileList, const char* pathToTestFile);
int addIfIsSingleTestCase(ArgList* argList);
void addSingleTestCaseToList(void* args[]);
void trimTestName(char *testName);

#ifdef __cplusplus
}
#endif  

#endif