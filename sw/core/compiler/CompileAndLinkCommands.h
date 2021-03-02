#ifndef COMPILE_AND_LINK_COMMANDS_H
#define COMPILE_AND_LINK_COMMANDS_H

#include "../../external/GregCToolkit/sw/ArgList/ArgList.h"
#include "../common/FileStructureDefs.h"

#ifdef __cplusplus
extern "C"
{
#endif

   int compileIntoTempObjectFiles(
       const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
   int compileIntoTempObjectFilesWithCompiler(
       const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, char* compiler, const char* basePath);
   int compileIntoObjectFiles(ArgList* compilerArgs, const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, char* compiler);
   int moveObjectFilesToTempDir(ArgList* mvArgs, const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, char* compiler);
   void populateTempObjectFileArgs(
       ObjectFileList* tempObjectFiles, ArgList* argList, const TestFileList* testFiles, const SourceFileList* sourceFiles, const char* compiler, int offset,
       int optionsOffset);
   int linkObjectFilesWithGregTestLibraryToMakeProjectTestLibrary(
       const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
   int createTestMainExecutableFromProjectLibraryAndGregTestLibrary(
       const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
   int compileIntoProjectExecutable(
       const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
   int linkObjectFiles(char* compiler, const ObjectFileList* tempObjectFiles);
   void getArgsForFileList(
       ObjectFileList* tempObjectFiles, int* argIndex, const void* fileList, ArgList* argList, const char* compiler, int offset, int optionsOffset, int listType);

#ifdef __cplusplus
}
#endif

#endif