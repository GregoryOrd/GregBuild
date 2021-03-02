#include "CompileAndLinkCommands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/ExternalProgramExecution/ExternalProgramExecution.h"
#include "../../external/GregCToolkit/sw/FailureHandling/FailureHandling.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/FileOperations.h"
#include "../common/GregBuildConstants.h"
#include "../common/global/GlobalVariables.h"
#include "CompileAndLinkHelpers.h"

int compileIntoTempObjectFiles(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);
   return compileIntoTempObjectFilesWithCompiler(testFiles, sourceFiles, tempObjectFiles, hostCompiler(), basePath);
}

int compileIntoTempObjectFilesWithCompiler(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, char* compiler, const char* basePath)
{
   ArgList* compilerArgs = malloc(sizeof(ArgList));
   ArgList* mvArgs = malloc(sizeof(ArgList));

   int error = 0;

   error = compileIntoObjectFiles(compilerArgs, testFiles, sourceFiles, tempObjectFiles, compiler);
   if (error)
   {
      printf("\n===================================================================\n");
      printf("Error compiling with compiler: %s\n", compiler);
      printf("===================================================================\n");
      setHostCompileFailed();
      resetObjectFileList(tempObjectFiles);
   }
   else
   {
      moveObjectFilesToTempDir(mvArgs, testFiles, sourceFiles, tempObjectFiles, compiler);
   }

   freeArgList(compilerArgs, true);
   freeArgList(mvArgs, true);
   return 0;
}

int compileIntoObjectFiles(ArgList* compilerArgs, const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, char* compiler)
{
   LinkedList* options;
   if (stringsAreEqual(compiler, hostCompiler()))
   {
      options = hostCompilerOptions();
   }
   else
   {
      options = targetCompilerOptions();
   }
   int numTestFiles = testFilesSize(testFiles);
   initArgsForCompilingToObjectFiles(compilerArgs, sourceFiles, numTestFiles, compiler);
   populateTempObjectFileArgs(tempObjectFiles, compilerArgs, testFiles, sourceFiles, compiler, gccFileArgOffset, options->size);
   return popenChildProcess(compilerArgs->size, (char* const*)compilerArgs->args);
}

int moveObjectFilesToTempDir(ArgList* mvArgs, const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, char* compiler)
{
   int numTestFiles = testFilesSize(testFiles);
   initMvArgsForMovingCompiledObjectFilesToTempDir(mvArgs, sourceFiles, numTestFiles, compiler);
   populateTempObjectFileArgs(tempObjectFiles, mvArgs, testFiles, sourceFiles, compiler, mvFileArgOffset, 0);
   return forkAndRunChildProcess((char* const*)mvArgs->args);
}

void populateTempObjectFileArgs(
    ObjectFileList* tempObjectFiles, ArgList* argList, const TestFileList* testFiles, const SourceFileList* sourceFiles, const char* compiler, int offset, int optionsOffset)
{
   resetObjectFileList(tempObjectFiles);

   int argIndex = 0;
   if (testFiles != NULL)
   {
      getArgsForFileList(tempObjectFiles, &argIndex, testFiles, argList, compiler, offset, optionsOffset, TEST_FILE_LIST_TYPE);
   }
   getArgsForFileList(tempObjectFiles, &argIndex, sourceFiles, argList, compiler, offset, optionsOffset, SRC_FILE_LIST_TYPE);
}

void printArgList(ArgList* argList)
{
   for (int i = 0; i < argList->size; i++)
   {
      printf("Args[%d]: %s\n", i, (char*)argList->args[i]);
   }
}

int linkObjectFilesWithGregTestLibraryToMakeProjectTestLibrary(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);
   if (tempObjectFiles->size == 0)
   {
      printf("\n===================================================================\n");
      printf("No host object files found, skipping tests\n");
      printf("===================================================================\n");
   }
   else if (tempObjectFiles->size > 0)
   {
      ArgList* linkerArgs = malloc(sizeof(ArgList));
      initArgsForLinkingTestExecutable(linkerArgs, tempObjectFiles, hostCompiler());
      fileArgsForLinkingTestExecutable(linkerArgs, tempObjectFiles);
      popenChildProcess(linkerArgs->size, (char* const*)linkerArgs->args);

      freeArgList(linkerArgs, true);
   }

   return 0;
}

int createTestMainExecutableFromProjectLibraryAndGregTestLibrary(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);
   if (tempObjectFiles->size > 0)
   {
      char* const argv[] = {hostCompiler(), "-o", TEMP_TEST_MAIN, TEMP_TEST_MAIN_C, "-L./", TEMP_TEST_PROJECT_LIBRARY, LIB_GREG_TEST_LIBRARY, NULL};
      return popenChildProcess(7, argv);
   }
   return 0;
}

int compileIntoProjectExecutable(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);

   if (!stringsAreEqual(hostCompiler(), targetCompiler()))
   {
      compileIntoTempObjectFilesWithCompiler(testFiles, sourceFiles, tempObjectFiles, targetCompiler(), basePath);
   }
   return linkObjectFiles(targetCompiler(), tempObjectFiles);
}

int linkObjectFiles(char* compiler, const ObjectFileList* tempObjectFiles)
{
   ArgList* linkerArgs = malloc(sizeof(ArgList));
   initArgsForLinkingProjectExecutable(linkerArgs, tempObjectFiles, compiler);
   fileArgsForLinkingProjectExecutable(linkerArgs, tempObjectFiles);
   makeDir(DIST);
   int retval = popenChildProcess(linkerArgs->size, (char* const*)linkerArgs->args);
   freeArgList(linkerArgs, true);
   if (retval == 0)
   {
      if (testsWereRun())
      {
         printf("\nBuild Successful!\n");
      }
      else
      {
         printf("\nBuild Successful. NO TESTS WERE RUN.\n");
      }
   }
   return retval;
}

void getArgsForFileList(
    ObjectFileList* tempObjectFiles, int* argIndex, const void* fileList, ArgList* argList, const char* compiler, int offset, int optionsOffset, int listType)
{
   char objectFileName[WINDOWS_MAX_PATH_LENGTH] = "";
   int index = 0;
   int numFiles = listSize(fileList, listType);
   while (index < numFiles)
   {
      determineObjectFileNameUsingListType(listType, objectFileName, fileList, index);
      addTempObjectFileToList(tempObjectFiles, objectFileName, compiler);
      copyTempObjectOrCFileNameIntoArgList(argList, argIndex, offset, optionsOffset, fileList, index, objectFileName, listType);

      (*argIndex)++;
      index++;
   }
}