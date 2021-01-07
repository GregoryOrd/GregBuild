#ifndef RUN_TESTS_H
#define RUN_TESTS_H 

#include "../fileSystemRecursion/ObjectFileStructureDefs.h"
#include "../fileSystemRecursion/SourceFileStructureDefs.h"
#include "../fileSystemRecursion/TestStructureDefs.h"

int runTests();
int runTestsWithExitStatusCheck(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath);

#endif