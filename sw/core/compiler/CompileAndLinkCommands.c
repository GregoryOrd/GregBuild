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
#include "../common/ObjAndTempDefs.h"
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
   int error = 0;

   error = compileIntoObjectFiles(testFiles, sourceFiles, tempObjectFiles, compiler);
   if (error)
   {
      printf("\n===================================================================\n");
      printf("Error compiling with compiler: %s\n", compiler);
      printf("===================================================================\n");
      setHostCompileFailed();
      resetObjectFileList(tempObjectFiles);
   }

   return 0;
}

int compileIntoObjectFiles(const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, char* compiler)
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
   int result = 0;
   char objectFileName[WINDOWS_MAX_PATH_LENGTH] = "";
   char tempObjectFile[WINDOWS_MAX_PATH_LENGTH] = "";

   bool sameCompiler = stringsAreEqual(hostCompiler(), targetCompiler());
   bool host = !sameCompiler && stringsAreEqual(hostCompiler(), compiler);
   bool target = !sameCompiler && stringsAreEqual(targetCompiler(), compiler);

   if (target)
   {
      resetObjectFileList(tempObjectFiles);
   }

   for (int i = 0; i < numTestFiles; i++)
   {
      bool hostAndNotExcluded = host && !contains_string_ll(hostExcludedFiles(), testFiles->files[i].name, HOST_EXCLUDED_FILE_TYPE);
      bool targetAndNotExcluded = target && !contains_string_ll(targetExcludedFiles(), testFiles->files[i].name, TARGET_EXCLUDED_FILE_TYPE);
      bool sameCompilerAndNotExcluded = sameCompiler && !contains_string_ll(hostExcludedFiles(), testFiles->files[i].name, HOST_EXCLUDED_FILE_TYPE) &&
                                        !contains_string_ll(targetExcludedFiles(), testFiles->files[i].name, TARGET_EXCLUDED_FILE_TYPE);
      if (hostAndNotExcluded || targetAndNotExcluded || sameCompilerAndNotExcluded)
      {
         ArgList* compilerArgs = malloc(sizeof(ArgList));
         determineObjectFileNameUsingListType(TEST_FILE_LIST_TYPE, objectFileName, testFiles, i);
         addTempObjectFileToList(tempObjectFiles, objectFileName, tempObjectFile, compiler);
         argsForCompilingToObjectFile(compilerArgs, testFiles->files[i].name, tempObjectFile, compiler);
         result |= popenChildProcess(compilerArgs->size, (char* const*)compilerArgs->args);
         freeArgList(compilerArgs, true);
      }
   }

   for (int i = 0; i < sourceFiles->size; i++)
   {
      bool hostAndNotExcluded = host && !contains_string_ll(hostExcludedFiles(), sourceFiles->files[i].name, HOST_EXCLUDED_FILE_TYPE);
      bool targetAndNotExcluded = target && !contains_string_ll(targetExcludedFiles(), sourceFiles->files[i].name, TARGET_EXCLUDED_FILE_TYPE);
      bool sameCompilerAndNotExcluded = sameCompiler && !contains_string_ll(hostExcludedFiles(), testFiles->files[i].name, HOST_EXCLUDED_FILE_TYPE) &&
                                        !contains_string_ll(targetExcludedFiles(), testFiles->files[i].name, TARGET_EXCLUDED_FILE_TYPE);
      if (hostAndNotExcluded || targetAndNotExcluded || sameCompilerAndNotExcluded)
      {
         ArgList* compilerArgs = malloc(sizeof(ArgList));
         determineObjectFileNameUsingListType(SRC_FILE_LIST_TYPE, objectFileName, sourceFiles, i);
         addTempObjectFileToList(tempObjectFiles, objectFileName, tempObjectFile, compiler);
         argsForCompilingToObjectFile(compilerArgs, sourceFiles->files[i].name, tempObjectFile, compiler);
         result |= popenChildProcess(compilerArgs->size, (char* const*)compilerArgs->args);
         freeArgList(compilerArgs, true);
      }
   }
   return result;
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