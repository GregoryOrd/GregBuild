#ifndef COMPILE_AND_LINK_COMMANDS_H
#define COMPILE_AND_LINK_COMMANDS_H

#include "../common/FileStructureDefs.h"

#ifdef __cplusplus
extern "C"
{
#endif

   int compileIntoTempObjectFiles(
       const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
   int compileIntoTempObjectFilesWithCompiler(
       const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, const char* compiler, const char* basePath);
   int compileIntoObjectFiles(const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, const char* compiler);
   int linkObjectFilesWithGregTestLibraryToMakeProjectTestLibrary(
       const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
   int createTestMainExecutableFromProjectLibraryAndGregTestLibrary(
       const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
   int compileIntoProjectExecutable(
       const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath);
   int linkObjectFiles(const char* compiler, const ObjectFileList* tempObjectFiles);

#ifdef __cplusplus
}
#endif

#endif