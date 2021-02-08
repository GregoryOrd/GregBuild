#include "CompileAndLinkCommands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/ExternalProgramExecution/CommandLineExecutables.h"
#include "../../external/GregCToolkit/sw/ExternalProgramExecution/ExternalProgramExecution.h"
#include "../../external/GregCToolkit/sw/FailureHandling/FailureHandling.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/GregBuildConstants.h"

int compileIntoTempObjectFiles(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);

   int testFilesSize = 0;
   if (testFiles != NULL)
   {
      testFilesSize = testFiles->size;
   }

   ArgList* gccArgs = malloc(sizeof(ArgList));
   gccArgs->size = testFilesSize + sourceFiles->size + 3;
   gccArgs->args = malloc(gccArgs->size * sizeof(void*));

   ArgList* mvArgs = malloc(sizeof(ArgList));
   mvArgs->size = testFilesSize + sourceFiles->size + 3;
   mvArgs->args = malloc(mvArgs->size * sizeof(void*));

   populateArgsFor_compileIntoTempObjectFiles(tempObjectFiles, gccArgs, mvArgs, testFiles, sourceFiles);
   forkAndRunChildProcess(gcc, (char* const*)gccArgs->args);
   forkAndRunChildProcess(mv, (char* const*)mvArgs->args);

   freeArgList(gccArgs);
   free(mvArgs);
   return 0;
}

void populateArgsFor_compileIntoTempObjectFiles(
    ObjectFileList* tempObjectFiles, ArgList* gccArgs, ArgList* mvArgs, const TestFileList* testFiles, const SourceFileList* sourceFiles)
{
   gccArgs->args[0] = gcc;
   gccArgs->args[1] = "-c";
   gccArgs->args[gccArgs->size - 1] = NULL;

   mvArgs->args[0] = mv;
   mvArgs->args[mvArgs->size - 2] = TEMP_DIR;
   mvArgs->args[mvArgs->size - 1] = NULL;

   int argIndex = 0;
   if (testFiles != NULL)
   {
      getArgsForTestFiles(tempObjectFiles, &argIndex, testFiles, gccArgs, mvArgs);
   }
   getArgsForSourceFiles(tempObjectFiles, &argIndex, sourceFiles, gccArgs, mvArgs);
}

int linkObjectFilesWithGregTestDllToMakeProjectTestDll(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);

   ArgList* gccArgs = malloc(sizeof(ArgList));
   gccArgs->size = tempObjectFiles->size + 7;
   gccArgs->args = malloc(gccArgs->size * sizeof(void*));

   gccArgs->args[0] = gcc;
   gccArgs->args[1] = "-shared";
   gccArgs->args[2] = "-o";
   gccArgs->args[3] = TEMP_TEST_PROJECT_DLL;
   for (int i = 0; i < tempObjectFiles->size; i++)
   {
      gccArgs->args[i + 4] = (&tempObjectFiles->files[i])->name;
   }
   gccArgs->args[gccArgs->size - 3] = "-L./";
   gccArgs->args[gccArgs->size - 2] = LIB_GREG_TEST_DLL;
   gccArgs->args[gccArgs->size - 1] = NULL;

   forkAndRunChildProcess(gcc, (char* const*)gccArgs->args);

   freeArgList(gccArgs);
   return 0;
}

int createTestMainExecutableFromProjectDllAndGregTestDll(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);
   char* const argv[] = {gcc, "-o", TEMP_TEST_MAIN, TEMP_TEST_MAIN_C, "-L./", TEMP_TEST_PROJECT_DLL, LIB_GREG_TEST_DLL, NULL};
   return forkAndRunChildProcess(gcc, argv);
}

int compileObjectFilesIntoProjectExecutable(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);

   ArgList* gccArgs = malloc(sizeof(ArgList));
   gccArgs->size = numObjectFilesFromSource(tempObjectFiles) + 4;
   gccArgs->args = malloc(gccArgs->size * sizeof(void*));

   gccArgs->args[0] = gcc;
   int numObjectFilesFromSourceAddedToArgsList = 0;
   for (int i = 0; i < tempObjectFiles->size; i++)
   {
      ObjectFile* file = &tempObjectFiles->files[i];
      if (file->isFromSource)
      {
         gccArgs->args[numObjectFilesFromSourceAddedToArgsList + 1] = file->name;
         numObjectFilesFromSourceAddedToArgsList++;
      }
   }
   gccArgs->args[gccArgs->size - 3] = "-o";
   gccArgs->args[gccArgs->size - 2] = PROJECT_EXE;
   gccArgs->args[gccArgs->size - 1] = NULL;

   makeDir(DIST);
   int retval = forkAndRunChildProcess(gcc, (char* const*)gccArgs->args);
   freeArgList(gccArgs);
   if (retval == 0)
   {
      printf("\nBuild Successful!\n");
   }
   else
   {
      printf("Error Compiling the Code After Tests Completed\n");
   }
   return retval;
}

void getArgsForTestFiles(ObjectFileList* tempObjectFiles, int* argIndex, const TestFileList* testFiles, ArgList* gccArgs, ArgList* mvArgs)
{
   char objectFileName[WINDOWS_MAX_PATH_LENGTH] = "";
   int gccFileArgOffset = 2;
   int mvFileArgOffset = 1;
   int testFileIndex = 0;
   while (testFileIndex < testFiles->size)
   {
      determineObjectFileName(objectFileName, testFiles->files[testFileIndex].name);
      addTempObjectFileToList(tempObjectFiles, objectFileName, false);

      gccArgs->args[*argIndex + gccFileArgOffset] = malloc(strlen(testFiles->files[testFileIndex].name));
      mvArgs->args[*argIndex + mvFileArgOffset] = malloc(strlen(objectFileName));

      strcpy(gccArgs->args[*argIndex + gccFileArgOffset], testFiles->files[testFileIndex].name);
      strcpy(mvArgs->args[*argIndex + mvFileArgOffset], objectFileName);
      (*argIndex)++;
      testFileIndex++;
   }
}

void getArgsForSourceFiles(ObjectFileList* tempObjectFiles, int* argIndex, const SourceFileList* sourceFiles, ArgList* gccArgs, ArgList* mvArgs)
{
   int gccFileArgOffset = 2;
   int mvFileArgOffset = 1;
   int sourceFileIndex = 0;
   char objectFileName[WINDOWS_MAX_PATH_LENGTH] = "";
   while (sourceFileIndex < sourceFiles->size)
   {
      determineObjectFileName(objectFileName, sourceFiles->files[sourceFileIndex].name);
      addTempObjectFileToList(tempObjectFiles, objectFileName, true);

      gccArgs->args[*argIndex + gccFileArgOffset] = malloc(strlen(sourceFiles->files[sourceFileIndex].name));
      mvArgs->args[*argIndex + mvFileArgOffset] = malloc(strlen(objectFileName));

      strcpy(gccArgs->args[*argIndex + gccFileArgOffset], sourceFiles->files[sourceFileIndex].name);
      strcpy(mvArgs->args[*argIndex + mvFileArgOffset], objectFileName);
      (*argIndex)++;
      sourceFileIndex++;
   }
}

void determineObjectFileName(char* objectFileName, const char* filePath)
{
   int length = strlen(filePath) - 1;
   int offset = 2;
   char reversedObjectFileName[WINDOWS_MAX_PATH_LENGTH] = "";
   reversedObjectFileName[0] = 'o';
   reversedObjectFileName[1] = '.';
   bool pastExtension = false;

   for (int i = length - 1; i > 0; i--)
   {
      if (filePath[i] == '\\' || filePath[i] == '/')
      {
         break;
      }
      else if (filePath[i] != '.')
      {
         reversedObjectFileName[offset] = filePath[i];
         reversedObjectFileName[offset + 1] = '\0';
         offset++;
      }
      else if (filePath[i] == '.')
      {
         pastExtension = true;
      }
   }
   reverseString(objectFileName, reversedObjectFileName);
}

void addTempObjectFileToList(ObjectFileList* list, const char* filename, bool isFromSource)
{
   char tempObjectFile[WINDOWS_MAX_PATH_LENGTH] = "";
   strcat(tempObjectFile, TEMP_DIR);
   strcat(tempObjectFile, "\\");
   strcat(tempObjectFile, filename);

   list->files = (ObjectFile*)realloc(list->files, ((list->size + 1) * sizeof(ObjectFile)));
   list->files[list->size].name = malloc(strlen(tempObjectFile));
   strcpy(list->files[list->size].name, tempObjectFile);
   list->files[list->size].isFromSource = isFromSource;
   list->size++;
}

int numObjectFilesFromSource(const ObjectFileList* tempObjectFiles)
{
   int count = 0;
   for (int i = 0; i < tempObjectFiles->size; i++)
   {
      if ((&tempObjectFiles->files[i])->isFromSource)
      {
         count++;
      }
   }
   return count;
}

void freeArgList(ArgList* argList)
{
   free(argList->args);
   free(argList);
}