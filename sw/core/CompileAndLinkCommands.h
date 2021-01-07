#ifndef COMPILE_AND_LINK_COMMANDS_H
#define COMPILE_AND_LINK_COMMANDS_H 

#include "../commandLineCalls/ArgListDefs.h"
#include "../fileSystemRecursion/ObjectFileStructureDefs.h"
#include "../fileSystemRecursion/SourceFileStructureDefs.h"
#include "../fileSystemRecursion/TestStructureDefs.h"

int compileIntoTempObjectFiles(ObjectFileList* tempObjectFiles, TestFileList* testCases, SourceFileList* sourceFiles);
void populateArgsFor_compileIntoTempObjectFiles(ObjectFileList* tempObjectFiles, ArgList* gccArgs, ArgList* mvArgs, TestFileList* testCases, SourceFileList* sourceFiles);
int linkObjectFilesWithGregTestDllToMakeProjectTestDll(ObjectFileList* tempObjectFiles);
int createTestMainExecutableFromProjectDllAndGregTestDll();
int compileObjectFilesIntoProjectExecutable();
void getArgsForTestFiles(ObjectFileList* tempObjectFiles, int* argIndex, TestFileList* testFiles, ArgList* gccArgs, ArgList* mvArgs);
void getArgsForSourceFiles(ObjectFileList* tempObjectFiles, int* argIndex, SourceFileList* sourceFiles, ArgList* gccArgs, ArgList* mvArgs);
void determineObjectFileName(char* objectFileName, const char* filePath);
void addTempObjectFileToList(ObjectFileList* list, char* filename, bool isFromSource);
int numObjectFilesFromSource(ObjectFileList* tempObjectFiles);
void freeArgList(ArgList* argList);

#endif