#ifndef RUN_TESTS_H
#define RUN_TESTS_H 

#include "../fileSystemRecursion/FileStructureDefs.h"

int runTests();
int runTestsWithExitStatusCheck(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath);
void copyDllsToCurrentDirectory();
void removeDllsFromCurrentDirectory();

#endif