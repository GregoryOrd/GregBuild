#ifndef BUILD_SEQUENCE_H
#define BUILD_SEQUENCE_H

#include "CompileAndLinkCommands.h"
#include "RunTests.h"
#include "../fileSystemRecursion/FileAndTestCaseGatherer.h"
#include "../testMainWriting/TestMainWriter.h"

#define NUM_FUNCTIONS_IN_SEQUENCE 7

int (*loadTestsAndSourceFiles_func_ptr)(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath) = &loadTestsAndSourceFiles;
int (*compileIntoTempObjectFiles_func_ptr)(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath) = &compileIntoTempObjectFiles;
int (*linkObjectFilesWithGregTestDllToMakeProjectTestDll_func_ptr)(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath) = &linkObjectFilesWithGregTestDllToMakeProjectTestDll;
int (*writeTestsToTestMain_func_ptr)(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath) = &writeTestsToTestMain;
int (*createTestMainExecutableFromProjectDllAndGregTestDll_func_ptr)(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath) = &createTestMainExecutableFromProjectDllAndGregTestDll;
int (*runTestsWithExitStatusCheck_func_ptr)(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath) = &runTestsWithExitStatusCheck;
int (*compileObjectFilesIntoProjectExecutable_func_ptr)(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath) = &compileObjectFilesIntoProjectExecutable;

int (*buildSequence[NUM_FUNCTIONS_IN_SEQUENCE])(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath) = {
    loadTestsAndSourceFiles,
    compileIntoTempObjectFiles,
    linkObjectFilesWithGregTestDllToMakeProjectTestDll,
    writeTestsToTestMain,
    createTestMainExecutableFromProjectDllAndGregTestDll,
    runTestsWithExitStatusCheck,
    compileObjectFilesIntoProjectExecutable
};
#endif