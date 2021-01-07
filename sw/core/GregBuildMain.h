#ifndef GREG_TEST_MAIN_H
#define GREG_TEST_MAIN _H

#include "CommandLineOptions.h"
#include "../fileSystemRecursion/TestStructureDefs.h"
#include "../fileSystemRecursion/SourceFileStructureDefs.h"
#include "../fileSystemRecursion/ObjectFileStructureDefs.h"

void initCommandLineOptions(CommandLineOptions* options);
void processCommandLineArgs(int arc, char* argv[], CommandLineOptions* options);
void initFileListsAndTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void freeObjectFileList(ObjectFileList* list);
void initObjectFileList(ObjectFileList* objectFiles);
void removeTempDirAndFreeFileLists(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);

#endif