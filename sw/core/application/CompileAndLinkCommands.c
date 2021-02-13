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
#include "../fileSystemRecursion/FileOperations.h"
#include "../fileSystemRecursion/TestAndSrcDefinitions.h"
#include "CompilerConfiguration.h"

int compileIntoTempObjectFiles(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);
   return compileIntoTempObjectFilesWithCompiler(testFiles, sourceFiles, tempObjectFiles, hostCompiler(), basePath);
}

int compileIntoTempObjectFilesWithCompiler(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, char* compiler, const char* basePath)
{
   ArgList* gccArgs = malloc(sizeof(ArgList));
   ArgList* mvArgs = malloc(sizeof(ArgList));

   compileIntoObjectFiles(gccArgs, testFiles, sourceFiles, tempObjectFiles, compiler);
   moveObjectFilesToTempDir(mvArgs, testFiles, sourceFiles, tempObjectFiles, compiler);

   freeArgList(gccArgs);
   free(mvArgs);
   return 0;
}

int compileIntoObjectFiles(ArgList* gccArgs, const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, char* compiler)
{
   int numTestFiles = testFilesSize(testFiles);
   initGccArgsForCompilerToObjectFiles(gccArgs, sourceFiles, numTestFiles, compiler);
   populateArgsFor_compileIntoTempObjectFiles(tempObjectFiles, gccArgs, testFiles, sourceFiles, compiler, gccFileArgOffset);
   return forkAndRunChildProcess(compiler, (char* const*)gccArgs->args);
}

int moveObjectFilesToTempDir(ArgList* mvArgs, const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, char* compiler)
{
   int numTestFiles = testFilesSize(testFiles);
   initMvArgsForCompilerToObjectFiles(mvArgs, sourceFiles, numTestFiles, compiler);
   populateArgsFor_compileIntoTempObjectFiles(tempObjectFiles, mvArgs, testFiles, sourceFiles, compiler, mvFileArgOffset);
   return forkAndRunChildProcess(mv, (char* const*)mvArgs->args);
}

void populateArgsFor_compileIntoTempObjectFiles(
    ObjectFileList* tempObjectFiles, ArgList* argList, const TestFileList* testFiles, const SourceFileList* sourceFiles, const char* compiler, int offset)
{
   resetObjectFileList(tempObjectFiles);

   int argIndex = 0;
   if (testFiles != NULL)
   {
      getArgsForFileList(tempObjectFiles, &argIndex, testFiles, argList, compiler, offset, TEST_FILE_LIST_TYPE);
   }
   getArgsForFileList(tempObjectFiles, &argIndex, sourceFiles, argList, compiler, offset, SRC_FILE_LIST_TYPE);
}

void printArgList(ArgList* argList)
{
   for (int i = 0; i < argList->size; i++)
   {
      printf("Args[%d]: %s\n", i, (char*)argList->args[i]);
   }
}

int linkObjectFilesWithGregTestDllToMakeProjectTestDll(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);

   ArgList* gccArgs = malloc(sizeof(ArgList));
   initGccArgsForCompileTestExecutable(gccArgs, tempObjectFiles, hostCompiler());
   fileArgsForCompileTestExecutable(gccArgs, tempObjectFiles);

   forkAndRunChildProcess(hostCompiler(), (char* const*)gccArgs->args);

   freeArgList(gccArgs);
   return 0;
}

int createTestMainExecutableFromProjectDllAndGregTestDll(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);
   char* const argv[] = {hostCompiler(), "-o", TEMP_TEST_MAIN, TEMP_TEST_MAIN_C, "-L./", TEMP_TEST_PROJECT_DLL, LIB_GREG_TEST_DLL, NULL};
   return forkAndRunChildProcess(hostCompiler(), argv);
}

int compileIntoProjectExecutable(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);

   if (strcmp(hostCompiler(), targetCompiler()) != 0)
   {
      compileIntoTempObjectFilesWithCompiler(testFiles, sourceFiles, tempObjectFiles, targetCompiler(), basePath);
   }
   return compileWithObjectFiles(targetCompiler(), tempObjectFiles);
}

int compileWithObjectFiles(char* compiler, const ObjectFileList* tempObjectFiles)
{
   ArgList* gccArgs = malloc(sizeof(ArgList));
   initGccArgsForCompileProjectExecutableFromObjectFiles(gccArgs, tempObjectFiles, compiler);
   fileArgsForCompileProjectExecutable(gccArgs, tempObjectFiles);
   printArgList(gccArgs);
   makeDir(DIST);
   int retval = forkAndRunChildProcess(compiler, (char* const*)gccArgs->args);
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

void getArgsForFileList(ObjectFileList* tempObjectFiles, int* argIndex, const void* fileList, ArgList* argList, const char* compiler, int offset, int listType)
{
   char objectFileName[WINDOWS_MAX_PATH_LENGTH] = "";
   int index = 0;
   int numFiles = listSize(fileList, listType);
   while (index < numFiles)
   {
      determineObjectFileNameUsingListType(listType, objectFileName, fileList, index);
      addTempObjectFileToList(tempObjectFiles, objectFileName, compiler);
      copyTempObjectOrCFileNameIntoArgList(argList, argIndex, offset, fileList, index, objectFileName, listType);

      (*argIndex)++;
      index++;
   }
}

void determineObjectFileNameUsingListType(int listType, char* objectFileName, const void* fileList, int index)
{
   if (listType == TEST_FILE_LIST_TYPE)
   {
      determineObjectFileName(objectFileName, ((TestFileList*)fileList)->files[index].name);
   }
   else
   {
      determineObjectFileName(objectFileName, ((SourceFileList*)fileList)->files[index].name);
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

void addTempObjectFileToList(ObjectFileList* list, const char* filename, const char* compiler)
{
   char tempObjectFile[WINDOWS_MAX_PATH_LENGTH] = "";
   tempDirPathFromCompiler(tempObjectFile, compiler);
   strcat(tempObjectFile, "\\");
   strcat(tempObjectFile, filename);

   list->files = (ObjectFile*)realloc(list->files, ((list->size + 1) * sizeof(ObjectFile)));
   list->files[list->size].name = malloc(strlen(tempObjectFile));
   strcpy(list->files[list->size].name, tempObjectFile);
   list->files[list->size].isFromSource = isObjectFileFromSourceFile(filename);
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

int testFilesSize(const TestFileList* testFiles)
{
   int testFilesSize = 0;
   if (testFiles != NULL)
   {
      testFilesSize = testFiles->size;
   }
   return testFilesSize;
}

void initGccArgsForCompileProjectExecutableFromObjectFiles(ArgList* gccArgs, const ObjectFileList* tempObjectFiles, char* compiler)
{
   gccArgs->size = numObjectFilesFromSource(tempObjectFiles) + 4;
   gccArgs->args = malloc(gccArgs->size * sizeof(void*));
   gccArgs->args[0] = compiler;

   gccArgs->args[gccArgs->size - 3] = "-o";
   gccArgs->args[gccArgs->size - 2] = PROJECT_EXE;
   gccArgs->args[gccArgs->size - 1] = NULL;
}

void fileArgsForCompileProjectExecutable(ArgList* gccArgs, const ObjectFileList* tempObjectFiles)
{
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
}

void initGccArgsForCompileTestExecutable(ArgList* gccArgs, const ObjectFileList* tempObjectFiles, char* compiler)
{
   gccArgs->size = tempObjectFiles->size + 7;
   gccArgs->args = malloc(gccArgs->size * sizeof(void*));
   for (int i = 0; i < gccArgs->size; i++)
   {
      gccArgs->args[i] = malloc(WINDOWS_MAX_PATH_LENGTH);
   }

   gccArgs->args[0] = compiler;
   gccArgs->args[1] = "-shared";
   gccArgs->args[2] = "-o";
   gccArgs->args[3] = TEMP_TEST_PROJECT_DLL;

   gccArgs->args[gccArgs->size - 3] = "-L./";
   gccArgs->args[gccArgs->size - 2] = LIB_GREG_TEST_DLL;
   gccArgs->args[gccArgs->size - 1] = NULL;
}

void fileArgsForCompileTestExecutable(ArgList* gccArgs, const ObjectFileList* tempObjectFiles)
{
   for (int i = 0; i < tempObjectFiles->size; i++)
   {
      gccArgs->args[i + 4] = (&tempObjectFiles->files[i])->name;
   }
}

void initGccArgsForCompilerToObjectFiles(ArgList* gccArgs, const SourceFileList* sourceFiles, int numTestFiles, char* compiler)
{
   gccArgs->size = numTestFiles + sourceFiles->size + 3;
   gccArgs->args = malloc(gccArgs->size * sizeof(void*));
   for (int i = 0; i < gccArgs->size; i++)
   {
      gccArgs->args[i] = malloc(WINDOWS_MAX_PATH_LENGTH);
   }
   gccArgs->args[0] = compiler;
   gccArgs->args[1] = "-c";
   gccArgs->args[gccArgs->size - 1] = NULL;
}

void initMvArgsForCompilerToObjectFiles(ArgList* mvArgs, const SourceFileList* sourceFiles, int numTestFiles, char* compiler)
{
   mvArgs->size = numTestFiles + sourceFiles->size + 3;
   mvArgs->args = malloc(mvArgs->size * sizeof(void*));
   mvArgs->args[0] = mv;
   for (int i = 0; i < mvArgs->size; i++)
   {
      mvArgs->args[i] = malloc(WINDOWS_MAX_PATH_LENGTH);
   }

   char tempFilePath[WINDOWS_MAX_PATH_LENGTH] = "";
   tempDirPathFromCompiler(tempFilePath, compiler);
   makeDir(tempFilePath);
   mvArgs->args[mvArgs->size - 2] = malloc(WINDOWS_MAX_PATH_LENGTH);
   strcpy(mvArgs->args[mvArgs->size - 2], tempFilePath);
   mvArgs->args[mvArgs->size - 1] = NULL;
}

void tempDirPathFromCompiler(char* dest, const char* compiler)
{
   char hardwarePlatform[7] = "target";
   if (strcmp(compiler, hostCompiler()) == 0)
   {
      strcpy(hardwarePlatform, "host");
   }

   clearString(dest);
   strcpy(dest, TEMP_DIR);
   strcat(dest, "\\");
   strcat(dest, hardwarePlatform);
}

int listSize(const void* fileList, int listType)
{
   int listSize = 0;
   if (listType == TEST_FILE_LIST_TYPE)
   {
      listSize = ((TestFileList*)fileList)->size;
   }
   else
   {
      listSize = ((SourceFileList*)fileList)->size;
   }
   return listSize;
}

void copyTempObjectOrCFileNameIntoArgList(ArgList* argList, int* argIndex, int offset, const void* fileList, int index, char* objectFileName, int listType)
{
   if (offset == mvFileArgOffset)
   {
      strcpy(argList->args[*argIndex + offset], objectFileName);
   }
   else if (offset == gccFileArgOffset)
   {
      strCopyUsingListType(listType, argList->args[*argIndex + offset], fileList, index);
   }
}

void strCopyUsingListType(int listType, char* dest, const void* fileList, int index)
{
   if (listType == TEST_FILE_LIST_TYPE)
   {
      strcpy(dest, ((TestFileList*)fileList)->files[index].name);
   }
   else
   {
      strcpy(dest, ((SourceFileList*)fileList)->files[index].name);
   }
}