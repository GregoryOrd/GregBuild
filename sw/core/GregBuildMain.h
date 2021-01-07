#ifndef GREG_TEST_MAIN_H
#define GREG_TEST_MAIN _H

#include "../fileSystemRecursion/TestStructureDefs.h"
#include "../fileSystemRecursion/SourceFileStructureDefs.h"
#include "../fileSystemRecursion/ObjectFileStructureDefs.h"

void exitIfPreviousStepFailed(int previousStepFailed);
void initFileListsAndTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void freeObjectFileList(ObjectFileList* list);
void initObjectFileList(ObjectFileList* objectFiles);
void removeTempDirAndFreeFileLists(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);

#endif