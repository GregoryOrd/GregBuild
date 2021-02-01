#ifndef RUN_TESTS_H
#define RUN_TESTS_H 

#include "../common/FileStructureDefs.h"

int runTests();
int runTestsWithExitStatusCheck(const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int previousStepFailed, const char* basePath);
void copyDllsToCurrentDirectory();
void removeDllsFromCurrentDirectory();

#endif