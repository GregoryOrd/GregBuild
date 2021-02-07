#ifndef COMPILE_AND_LINK_COMMANDS_H
#define COMPILE_AND_LINK_COMMANDS_H 

#include "../common/FileStructureDefs.h"
#include "../../external/GregCToolkit/sw/ExternalProgramExecution/ArgListDefs.h"

#ifdef __cplusplus
extern "C" {
#endif    

int compileIntoTempObjectFiles(const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
void populateArgsFor_compileIntoTempObjectFiles(ObjectFileList* tempObjectFiles, ArgList* gccArgs, ArgList* mvArgs, const TestFileList* testCases, const SourceFileList* sourceFiles);
int linkObjectFilesWithGregTestDllToMakeProjectTestDll(const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
int createTestMainExecutableFromProjectDllAndGregTestDll(const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
int compileObjectFilesIntoProjectExecutable(const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
void getArgsForTestFiles(ObjectFileList* tempObjectFiles, int* argIndex, const TestFileList* testFiles, ArgList* gccArgs, ArgList* mvArgs);
void getArgsForSourceFiles(ObjectFileList* tempObjectFiles, int* argIndex, const SourceFileList* sourceFiles, ArgList* gccArgs, ArgList* mvArgs);
void determineObjectFileName(char* objectFileName, const char* filePath);
void addTempObjectFileToList(ObjectFileList* list, char* filename, bool isFromSource);
int numObjectFilesFromSource(const ObjectFileList* tempObjectFiles);
void freeArgList(ArgList* argList);

#ifdef __cplusplus
}
#endif 

#endif