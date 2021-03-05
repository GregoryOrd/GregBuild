#ifndef COMPILE_AND_LINK_HELPERS_H
#define COMPILER_AND_LINK_HELPERS_H

#include "../../external/GregCToolkit/sw/ArgList/ArgList.h"
#include "../common/FileStructureDefs.h"

#define gccFileArgOffset 2
#define mvFileArgOffset  1

#ifdef __cplusplus
extern "C" {
#endif

void addTempObjectFileToList(ObjectFileList* list, const char* filename, const char* compiler);
int numObjectFilesFromSource(const ObjectFileList* tempObjectFiles);
int testFilesSize(const TestFileList* testFiles);
void initArgsForLinkingProjectExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles, char* compiler);
void fileArgsForLinkingProjectExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles);
void initArgsForLinkingTestExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles, char* compiler);
void fileArgsForLinkingTestExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles);
void initArgsForCompilingToObjectFiles(ArgList* compilerArgs, const SourceFileList* sourceFiles, int numTestFiles, char* compiler);
void initMvArgsForMovingCompiledObjectFilesToTempDir(ArgList* mvArgs, const SourceFileList* sourceFiles, int numTestFiles, char* compiler);
int listSize(const void* fileList, int listType);
void copyTempObjectOrCFileNameIntoArgList(ArgList* argList, int* argIndex, int offset, int optionsOffset, const void* fileList, int index, char* objectFileName, int listType);
void strCopyUsingListType(int listType, char* dest, const void* fileList, int index);
void printArgList(ArgList* argList);

#ifdef __cplusplus
}
#endif

#endif