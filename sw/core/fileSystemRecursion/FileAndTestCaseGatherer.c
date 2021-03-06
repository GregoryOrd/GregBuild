#include "FileAndTestCaseGatherer.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions.h"
#include "../../external/GregCToolkit/sw/FailureHandling/FailureHandling.h"
#include "../../external/GregCToolkit/sw/FileSystem/FileIO/FileReader.h"
#include "../../external/GregCToolkit/sw/FileSystem/FileSystemRecurser.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../common/FileOperations.h"
#include "../common/GregBuildConstants.h"
#include "../common/TestAndSrcDefinitions.h"

int loadTestsAndSourceFiles(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);

   int numArgs = 3;
   ArgList* argList = malloc(sizeof(ArgList));
   argList->size = numArgs;
   argList->args = calloc(numArgs, sizeof(void*));
   argList->args[0] = testFiles;
   argList->args[1] = sourceFiles;
   argList->args[2] = tempObjectFiles;
   recurseAndAddFilesToList(basePath, addToTestOrSourceList, argList);
   freeArgList(argList, false);

   return 0;
}

bool addToTestOrSourceList(ArgList* list, const char* basePath, const struct dirent* fileOrSubDirectory, const char* fileOrSubDirectoryFullPath)
{
   TestFileList* testFiles = (TestFileList*)list->args[0];
   SourceFileList* sourceFiles = (SourceFileList*)list->args[1];
   ObjectFileList* tempObjectFiles = (ObjectFileList*)list->args[2];

   bool testDir = isTestDir(basePath);
   bool testFile = isTestFile(fileOrSubDirectory);
   if (isTestDir(basePath) && isTestFile(fileOrSubDirectory))
   {
      addTestFileToList(testFiles, fileOrSubDirectoryFullPath);
      return true;
   }
   else if (!isVisibleDirectory(fileOrSubDirectory) && isSourceFile(fileOrSubDirectory->d_name))
   {
      addSourceFileToList(sourceFiles, fileOrSubDirectoryFullPath);
      return true;
   }

   return false;
}

void copyFileOrSubDirectoryNameIntoPath(char* path, const char* basePath, const char* fileOrSubDirectoryName)
{
   strcpy(path, basePath);
   strcat(path, DELIMITER);
   strcat(path, fileOrSubDirectoryName);
}

void addSourceFileToList(SourceFileList* list, const char* pathToSourceFile)
{
   if (list != NULL)
   {
      list->files = (SourceFile*)realloc(list->files, ((list->size + 1) * sizeof(SourceFile)));
      list->files[list->size].name = calloc(WINDOWS_MAX_PATH_LENGTH, sizeof(char));
      strcpy(list->files[list->size].name, pathToSourceFile);
      list->size++;
   }
}

void addTestFileToList(TestFileList* testFileList, const char* pathToTestFile)
{
   if (testFileList != NULL)
   {
      testFileList->files = (TestFile*)realloc(testFileList->files, (testFileList->size + 1) * sizeof(TestFile));
      testFileList->files[testFileList->size].name = calloc(WINDOWS_MAX_PATH_LENGTH, sizeof(char));
      testFileList->files[testFileList->size].numTestCases = 0;
      testFileList->files[testFileList->size].cases = malloc(sizeof(TestCase));
      strcpy(testFileList->files[testFileList->size].name, pathToTestFile);

      testFileList->size++;
   }
}