#ifndef COMPILE_AND_LINK_COMMANDS_H
#define COMPILE_AND_LINK_COMMANDS_H 

#include "../common/FileStructureDefs.h"
#include "../../external/GregCToolkit/sw/ArgList/ArgList.h"

#define gccFileArgOffset 2
#define mvFileArgOffset 1

#ifdef __cplusplus
extern "C" {
#endif    

int compileIntoTempObjectFiles(const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
int compileIntoTempObjectFilesWithCompiler(const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, char* compiler, const char* basePath);
int compileIntoObjectFiles(ArgList* compilerArgs, const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, char* compiler);
int moveObjectFilesToTempDir(ArgList* mvArgs, const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, char* compiler);
void populateTempObjectFileArgs(ObjectFileList* tempObjectFiles, ArgList* argList, const TestFileList* testFiles, const SourceFileList* sourceFiles, const char* compiler, int offset, int optionsOffset);
int linkObjectFilesWithGregTestLibraryToMakeProjectTestLibrary(const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
int createTestMainExecutableFromProjectLibraryAndGregTestLibrary(const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
int compileIntoProjectExecutable(const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
int linkObjectFiles(char* compiler, const ObjectFileList* tempObjectFiles);
void getArgsForFileList(ObjectFileList* tempObjectFiles, int* argIndex, const void* fileList, ArgList* argList, const char* compiler, int offset, int optionsOffset, int listType);

void determineObjectFileNameUsingListType(int listType, char* objectFileName, const void* fileList, int index);
void determineObjectFileName(char* objectFileName, const char* filePath);
void addTempObjectFileToList(ObjectFileList* list, const char* filename, const char* compiler);
int numObjectFilesFromSource(const ObjectFileList* tempObjectFiles);
int testFilesSize(const TestFileList* testFiles);
void initArgsForLinkingProjectExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles, char* compiler);
void fileArgsForLinkingProjectExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles);
void initArgsForLinkingTestExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles, char* compiler);
void fileArgsForLinkingTestExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles);
void initArgsForCompilingToObjectFiles(ArgList* compilerArgs, const SourceFileList* sourceFiles, int numTestFiles, char* compiler);
void initMvArgsForMovingCompiledObjectFilesToTempDir(ArgList* mvArgs, const SourceFileList* sourceFiles, int numTestFiles, char* compiler);
void tempDirPathFromCompiler(char* dest, const char* compiler);
int listSize(const void* fileList, int listType);
void copyTempObjectOrCFileNameIntoArgList(ArgList* argList, int* argIndex, int offset, int optionsOffset, const void* fileList, int index, char* objectFileName, int listType);
void strCopyUsingListType(int listType, char* dest, const void* fileList, int index);
void printArgList(ArgList* argList);


#ifdef __cplusplus
}
#endif 

#endif