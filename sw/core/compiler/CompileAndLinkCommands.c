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

//////////////////////////////////////////////////////////////////////
//              Private Data and Function Prototypes                //
//////////////////////////////////////////////////////////////////////
LinkedList* determineOptionsListFromCompiler(const char* compiler);
int compileTestFiles(const char* compiler, const TestFileList* testFiles, ObjectFileList* tempObjectFiles);
int compileSourceFiles(const char* compiler, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void resetObjectFileListForTarget(const char* compiler, ObjectFileList* tempObjectFiles);
int compileFileAndAddToTempObjectList(int fileListType, const void* list, int index, char* compiler, const char* fileName, ObjectFileList* tempObjectFiles);

//////////////////////////////////////////////////////////////////////
//              Function Implementation Section                     //
//////////////////////////////////////////////////////////////////////

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
   resetObjectFileListForTarget(compiler, tempObjectFiles);
   LinkedList* options = determineOptionsListFromCompiler(compiler);
   int result = 0;

   result |= compileTestFiles(compiler, testFiles, tempObjectFiles);
   if (result)
   {
      return result;
   }

   result |= compileSourceFiles(compiler, sourceFiles, tempObjectFiles);

   return result;
}

void resetObjectFileListForTarget(const char* compiler, ObjectFileList* tempObjectFiles)
{
   bool sameCompiler = stringsAreEqual(hostCompiler(), targetCompiler());

   if (!sameCompiler && stringsAreEqual(targetCompiler(), compiler))
   {
      resetObjectFileList(tempObjectFiles);
   }
}

int compileFileAndAddToTempObjectList(int fileListType, const void* list, int index, char* compiler, const char* fileName, ObjectFileList* tempObjectFiles)
{
   char objectFileName[WINDOWS_MAX_PATH_LENGTH] = "";
   char tempObjectFile[WINDOWS_MAX_PATH_LENGTH] = "";
   ArgList* compilerArgs = malloc(sizeof(ArgList));
   determineObjectFileNameUsingListType(fileListType, objectFileName, list, index);
   addTempObjectFileToList(tempObjectFiles, objectFileName, tempObjectFile, compiler);
   argsForCompilingToObjectFile(compilerArgs, fileName, tempObjectFile, compiler);
   int result = popenChildProcess(compilerArgs->size, (char* const*)compilerArgs->args);
   freeArgList(compilerArgs, true);
   return result;
}

int compileTestFiles(const char* compiler, const TestFileList* testFiles, ObjectFileList* tempObjectFiles)
{
   int result = 0;
   if (testBuild())
   {
      bool sameCompiler = stringsAreEqual(hostCompiler(), targetCompiler());
      bool host = !sameCompiler && stringsAreEqual(hostCompiler(), compiler);
      int numTestFiles = testFilesSize(testFiles);
      for (int i = 0; i < numTestFiles; i++)
      {
         bool hostAndNotExcluded = host && !contains_string_ll(hostExcludedFiles(), testFiles->files[i].name, HOST_EXCLUDED_FILE_TYPE);
         bool sameCompilerAndNotExcluded = sameCompiler && !contains_string_ll(hostExcludedFiles(), testFiles->files[i].name, HOST_EXCLUDED_FILE_TYPE) &&
                                           !contains_string_ll(targetExcludedFiles(), testFiles->files[i].name, TARGET_EXCLUDED_FILE_TYPE);
         // Only host or sameCompiler because we don't want to compile tests for the target
         if (hostAndNotExcluded || sameCompilerAndNotExcluded)
         {
            result |= compileFileAndAddToTempObjectList(TEST_FILE_LIST_TYPE, testFiles, i, compiler, testFiles->files[i].name, tempObjectFiles);
         }
      }
   }
   else
   {
      printf("No Test Build. Omitting test files from compilation.\n");
   }
   return result;
}

int compileSourceFiles(const char* compiler, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles)
{
   int result = 0;
   bool sameCompiler = stringsAreEqual(hostCompiler(), targetCompiler());
   bool host = !sameCompiler && stringsAreEqual(hostCompiler(), compiler);
   bool target = !sameCompiler && stringsAreEqual(targetCompiler(), compiler);

   for (int i = 0; i < sourceFiles->size; i++)
   {
      bool hostAndNotExcluded = host && !contains_string_ll(hostExcludedFiles(), sourceFiles->files[i].name, HOST_EXCLUDED_FILE_TYPE);
      bool targetAndNotExcluded = target && !contains_string_ll(targetExcludedFiles(), sourceFiles->files[i].name, TARGET_EXCLUDED_FILE_TYPE);
      bool sameCompilerAndNotExcluded = sameCompiler && !contains_string_ll(hostExcludedFiles(), sourceFiles->files[i].name, HOST_EXCLUDED_FILE_TYPE) &&
                                        !contains_string_ll(targetExcludedFiles(), sourceFiles->files[i].name, TARGET_EXCLUDED_FILE_TYPE);
      if (hostAndNotExcluded || targetAndNotExcluded || sameCompilerAndNotExcluded)
      {
         result |= compileFileAndAddToTempObjectList(SRC_FILE_LIST_TYPE, sourceFiles, i, compiler, sourceFiles->files[i].name, tempObjectFiles);
      }
   }
   return result;
}

LinkedList* determineOptionsListFromCompiler(const char* compiler)
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
   return options;
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
      if (testBuild())
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