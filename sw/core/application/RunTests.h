#ifndef RUN_TESTS_H
#define RUN_TESTS_H 

#include "../common/FileStructureDefs.h"

#ifdef __cplusplus
extern "C" {
#endif  

int runTests();
int runTestsWithExitStatusCheck(const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
void copyDllsToCurrentDirectory();
void removeDllsFromCurrentDirectory();

#ifdef __cplusplus
}
#endif  

#endif