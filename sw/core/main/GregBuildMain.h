#ifndef GREG_TEST_MAIN_H
#define GREG_TEST_MAIN _H

#include "../application/CommandLineOptions.h"
#include "../fileSystemRecursion/TestStructureDefs.h"
#include "../fileSystemRecursion/SourceFileStructureDefs.h"
#include "../fileSystemRecursion/ObjectFileStructureDefs.h"

int executeBuildSequence(CommandLineOptions* options, TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void exitIfPreviousStepFailed(int previousStepFailed);
void initAndProcessCommandLineOptions(CommandLineOptions* options, int argc, char* argv[]);
void initFileListsAndTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void freeObjectFileList(ObjectFileList* list);
void initObjectFileList(ObjectFileList* objectFiles);
void removeTempDirAndFreeFileLists(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);

#endif