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
static int compileTestFiles(const char* compiler, const TestFileList* testFiles, ObjectFileList* tempObjectFiles);
static int compileSourceFiles(const char* compiler, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
static void resetObjectFileListForTarget(const char* compiler, ObjectFileList* tempObjectFiles);
static int compileFileAndAddToTempObjectList(int fileListType, const void* list, int index, const char* compiler, const char* fileName, ObjectFileList* tempObjectFiles);
static bool sameCompiler();
static bool isHost(const char* compiler);
static bool isTarget(const char* compiler);
static bool hostAndNotExcluded(const char* compiler, const char* fileName);
static bool targetAndNotExcluded(const char* compiler, const char* fileName);
static bool sameCompilerAndNotExcluded(const char* compiler, const char* fileName);
static bool hostAndNotExcludedOrSameCompilerAndNotExcluded(const char* compiler, const char* fileName);
static bool hostOrTargetOrSameCompilerAndNotExcluded(const char* compiler, const char* fileName);

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
    const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, const char* compiler, const char* basePath)
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

int compileIntoObjectFiles(const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, const char* compiler)
{
   resetObjectFileListForTarget(compiler, tempObjectFiles);
   LinkedList* options = determineCompilerOptionsListFromCompiler(compiler);
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

int compileFileAndAddToTempObjectList(int fileListType, const void* list, int index, const char* compiler, const char* fileName, ObjectFileList* tempObjectFiles)
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

bool sameCompiler() { return stringsAreEqual(hostCompiler(), targetCompiler()); }
bool isHost(const char* compiler) { return !sameCompiler() && stringsAreEqual(hostCompiler(), compiler); }
bool isTarget(const char* compiler) { return !sameCompiler() && stringsAreEqual(targetCompiler(), compiler); }
bool hostAndNotExcluded(const char* compiler, const char* fileName) { return isHost(compiler) && !contains_string_ll(hostExcludedFiles(), fileName, HOST_EXCLUDED_FILE_TYPE); }
bool targetAndNotExcluded(const char* compiler, const char* fileName)
{
   return isTarget(compiler) && !contains_string_ll(targetExcludedFiles(), fileName, TARGET_EXCLUDED_FILE_TYPE);
}
bool sameCompilerAndNotExcluded(const char* compiler, const char* fileName)
{
   return sameCompiler() && !contains_string_ll(hostExcludedFiles(), fileName, HOST_EXCLUDED_FILE_TYPE) &&
          !contains_string_ll(targetExcludedFiles(), fileName, TARGET_EXCLUDED_FILE_TYPE);
}
bool hostAndNotExcludedOrSameCompilerAndNotExcluded(const char* compiler, const char* fileName)
{
   return hostAndNotExcluded(compiler, fileName) || sameCompilerAndNotExcluded(compiler, fileName);
}
bool hostOrTargetOrSameCompilerAndNotExcluded(const char* compiler, const char* fileName)
{
   return hostAndNotExcluded(compiler, fileName) || targetAndNotExcluded(compiler, fileName) || sameCompilerAndNotExcluded(compiler, fileName);
}

int compileTestFiles(const char* compiler, const TestFileList* testFiles, ObjectFileList* tempObjectFiles)
{
   int result = 0;
   if (testBuild())
   {
      for (int i = 0; i < testFilesSize(testFiles); i++)
      {
         const char* fileName = testFiles->files[i].name;
         if (hostAndNotExcludedOrSameCompilerAndNotExcluded(compiler, fileName))
         {
            result |= compileFileAndAddToTempObjectList(TEST_FILE_LIST_TYPE, testFiles, i, compiler, fileName, tempObjectFiles);
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
   for (int i = 0; i < sourceFiles->size; i++)
   {
      const char* fileName = sourceFiles->files[i].name;
      if (hostOrTargetOrSameCompilerAndNotExcluded(compiler, fileName))
      {
         result |= compileFileAndAddToTempObjectList(SRC_FILE_LIST_TYPE, sourceFiles, i, compiler, sourceFiles->files[i].name, tempObjectFiles);
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
      char* const argv[] = {(char*)hostCompiler(), "-o", TEMP_TEST_MAIN, TEMP_TEST_MAIN_C, "-L./", TEMP_TEST_PROJECT_LIBRARY, (char*)testFrameworkLibrary(), NULL};
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

int linkObjectFiles(const char* compiler, const ObjectFileList* tempObjectFiles)
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