#ifndef GREG_TEST_MAIN_H
#define GREG_TEST_MAIN _H

#include "../fileSystemRecursion/TestStructureDefs.h"
#include "../fileSystemRecursion/SourceFileStructureDefs.h"
#include "../fileSystemRecursion/ObjectFileStructureDefs.h"
#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions.h"

int executeBuildSequence(CommandLineOptionList* options, TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void exitIfPreviousStepFailed(int previousStepFailed);
void initAndProcessCommandLineOptions(CommandLineOptionList* options, int argc, char* argv[]);
void setCoreCommandLineOptions(CommandLineOptionList* list);
void coreCommandLineAcknowldegmentPrintouts(CommandLineOptionList* list);
void initFileListsAndTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void freeObjectFileList(ObjectFileList* list);
void initObjectFileList(ObjectFileList* objectFiles);
void freeFileLists(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
int removeTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath);

#endif