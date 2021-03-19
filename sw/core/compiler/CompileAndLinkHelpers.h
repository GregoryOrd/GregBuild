#ifndef COMPILE_AND_LINK_HELPERS_H
#define COMPILER_AND_LINK_HELPERS_H

#include "../../external/GregCToolkit/sw/ArgList/ArgList.h"
#include "../../external/GregCToolkit/sw/Collections/LinkedList/LinkedList.h"
#include "../common/FileStructureDefs.h"

#define gccFileArgOffset 2
#define mvFileArgOffset  1

#ifdef __cplusplus
extern "C" {
#endif

void addTempObjectFileToList(ObjectFileList* list, const char* filename, char* tempObjectFile, const char* compiler);
int numObjectFilesFromSource(const ObjectFileList* tempObjectFiles);
int testFilesSize(const TestFileList* testFiles);
void initArgsForLinkingProjectExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles, const char* compiler);
void fileArgsForLinkingProjectExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles);
void initArgsForLinkingTestExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles, const char* compiler);
void fileArgsForLinkingTestExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles);
LinkedList* determineOptionsListFromCompiler(const char* compiler);
void argsForCompilingToObjectFile(ArgList* compilerArgs, const char* filename, const char* tempObjectFileName, const char* compiler);
int listSize(const void* fileList, int listType);
void printArgList(ArgList* argList);

#ifdef __cplusplus
}
#endif

#endif