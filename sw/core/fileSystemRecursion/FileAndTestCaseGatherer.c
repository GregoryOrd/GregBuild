#include "FileAndTestCaseGatherer.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions.h"
#include "../../external/GregCToolkit/sw/FailureHandling/FailureHandling.h"
#include "../../external/GregCToolkit/sw/FileIO/FileReader.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "FileOperations.h"
#include "TestAndSrcDefinitions.h"

int loadTestsAndSourceFiles(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);

   char fileOrSubDirectoryFullPath[WINDOWS_MAX_PATH_LENGTH] = "";
   const struct dirent* fileOrSubDirectory;

   DIR* basePathDirectory = opendir(basePath);
   if (!basePathDirectory)
   {
      return 1;
   }

   while ((fileOrSubDirectory = readdir(basePathDirectory)) != NULL)
   {
      copyFileOrSubDirectoryNameIntoPath(fileOrSubDirectoryFullPath, basePath, fileOrSubDirectory->d_name);
      addToListOrEnterSubDirectoryForRecursion(testFiles, sourceFiles, tempObjectFiles, errorOnPreviousStep, basePath, fileOrSubDirectory, fileOrSubDirectoryFullPath);
   }

   closedir(basePathDirectory);
   return 0;
}

void addToListOrEnterSubDirectoryForRecursion(
    TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath,
    const struct dirent* fileOrSubDirectory, const char* fileOrSubDirectoryFullPath)
{
   if (isTestDir(basePath) && isTestFile(fileOrSubDirectory))
   {
      addTestFileToList(testFiles, fileOrSubDirectoryFullPath);
   }
   else if (!isVisibleDirectory(fileOrSubDirectory) && isSourceFile(fileOrSubDirectory->d_name))
   {
      addSourceFileToList(sourceFiles, fileOrSubDirectoryFullPath);
   }
   else if (isVisibleDirectory(fileOrSubDirectory))
   {
      loadTestsAndSourceFiles(testFiles, sourceFiles, tempObjectFiles, errorOnPreviousStep, fileOrSubDirectoryFullPath);
   }
}

void copyFileOrSubDirectoryNameIntoPath(char* path, const char* basePath, const char* fileOrSubDirectoryName)
{
   strcpy(path, basePath);
   strcat(path, "/");
   strcat(path, fileOrSubDirectoryName);
}

void addSourceFileToList(SourceFileList* list, const char* pathToSourceFile)
{
   if (list != NULL)
   {
      list->files = (SourceFile*)realloc(list->files, ((list->size + 1) * sizeof(SourceFile)));
      list->files[list->size].name = malloc(strlen(pathToSourceFile));
      strcpy(list->files[list->size].name, pathToSourceFile);
      list->size++;
   }
}

void addTestFileToList(TestFileList* testFileList, const char* pathToTestFile)
{
   if (testFileList != NULL)
   {
      testFileList->files = (TestFile*)realloc(testFileList->files, (testFileList->size + 1) * sizeof(TestFile));
      testFileList->files[testFileList->size].name = malloc(strlen(pathToTestFile));
      testFileList->files[testFileList->size].numTestCases = 0;
      testFileList->files[testFileList->size].cases = malloc(sizeof(TestCase));
      strcpy(testFileList->files[testFileList->size].name, pathToTestFile);

      addTestCasesToList(testFileList, pathToTestFile);

      testFileList->size++;
   }
}

void addTestCasesToList(TestFileList* testFileList, const char* pathToTestFile)
{
   ArgList* argsList = malloc(sizeof(ArgList));
   argsList->size = 1;
   argsList->args = malloc(argsList->size * sizeof(char*));
   argsList->args[0] = (void*)testFileList;

   readFileWithActionAfterEachLine(pathToTestFile, argsList, addIfIsSingleTestCase);
}

int addIfIsSingleTestCase(ArgList* argList)
{
   char* buffer = (char*)argList->args[argList->size - 1];
   if (isTestCaseDefinition(buffer))
   {
      addSingleTestCaseToList(argList->args);
   }

   return 0;
}

void addSingleTestCaseToList(void* args[])
{
   TestFileList* testFileList = (TestFileList*)args[0];
   char buffer[WINDOWS_MAX_PATH_LENGTH];
   strcpy(buffer, (char*)args[1]);
   trimTestName(buffer);

   TestFile* testFile = &testFileList->files[testFileList->size];
   testFile->cases = (TestCase*)realloc(testFile->cases, ((testFile->numTestCases + 1) * sizeof(TestCase)));
   testFile->cases[testFile->numTestCases].testName = malloc(strlen(buffer));

   strcpy(testFile->cases[testFile->numTestCases].testName, buffer);
   testFile->numTestCases++;
   testFileList->totalNumTestCases++;
}