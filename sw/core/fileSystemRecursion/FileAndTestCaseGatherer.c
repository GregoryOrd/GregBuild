#include "FileAndTestCaseGatherer.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions.h"
#include "../../external/GregCToolkit/sw/FailureHandling/FailureHandling.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "FileOperations.h"
#include "TestAndSrcDefinitions.h"

int loadTestsAndSourceFiles(TestFileList *testFiles,
                            SourceFileList *sourceFiles,
                            ObjectFileList *tempObjectFiles,
                            int previousStepFailed, const char *basePath) {
  exitIfPreviousStepFailed(previousStepFailed);

  char *fileOrSubDirectoryFullPath =
      (char *)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char *));
  const struct dirent *fileOrSubDirectory;

  DIR *basePathDirectory = opendir(basePath);
  if (!basePathDirectory) {
    return 1;
  }

  while ((fileOrSubDirectory = readdir(basePathDirectory)) != NULL) {
    copyFileOrSubDirectoryNameIntoPath(fileOrSubDirectoryFullPath, basePath,
                                       fileOrSubDirectory->d_name);
    addToListOrEnterSubDirectoryForRecursion(
        testFiles, sourceFiles, tempObjectFiles, previousStepFailed, basePath,
        fileOrSubDirectory, fileOrSubDirectoryFullPath);
  }

  closedir(basePathDirectory);
  free(fileOrSubDirectoryFullPath);
  return 0;
}

void addToListOrEnterSubDirectoryForRecursion(
    TestFileList *testFiles, SourceFileList *sourceFiles,
    ObjectFileList *tempObjectFiles, int previousStepFailed,
    const char *basePath, const struct dirent *fileOrSubDirectory,
    const char *fileOrSubDirectoryFullPath) {
  if (isTestDir(basePath) && isTestFile(fileOrSubDirectory)) {
    addTestFileToList(testFiles, fileOrSubDirectoryFullPath);
  } else if (!isVisibleDirectory(fileOrSubDirectory) &&
             isSourceFile(fileOrSubDirectory)) {
    addSourceFileToList(sourceFiles, fileOrSubDirectoryFullPath);
  } else if (isVisibleDirectory(fileOrSubDirectory)) {
    loadTestsAndSourceFiles(testFiles, sourceFiles, tempObjectFiles,
                            previousStepFailed, fileOrSubDirectoryFullPath);
  }
}

void copyFileOrSubDirectoryNameIntoPath(char *path, const char *basePath,
                                        const char *fileOrSubDirectoryName) {
  strcpy(path, basePath);
  strcat(path, "/");
  strcat(path, fileOrSubDirectoryName);
}

void addSourceFileToList(SourceFileList *list, const char *pathToSourceFile) {
  if (list != NULL) {
    list->files = (SourceFile *)realloc(
        list->files, ((list->size + 1) * sizeof(SourceFile)));
    list->files[list->size].name =
        (char *)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char *));
    strcpy(list->files[list->size].name, pathToSourceFile);
    list->size++;
  }
}

void addTestFileToList(TestFileList *testFileList, const char *pathToTestFile) {
  if (testFileList != NULL) {
    testFileList->files = (TestFile *)realloc(
        testFileList->files, (testFileList->size + 1) * sizeof(TestFile));
    testFileList->files[testFileList->size].name =
        (char *)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char *));
    testFileList->files[testFileList->size].numTestCases = 0;
    testFileList->files[testFileList->size].cases =
        (TestCase *)malloc(sizeof(TestCase));
    strcpy(testFileList->files[testFileList->size].name, pathToTestFile);

    addTestCasesToList(testFileList, pathToTestFile);

    testFileList->size++;
  }
}

void addTestCasesToList(TestFileList *testFileList,
                        const char *pathToTestFile) {
  FILE *testFilePtr;
  char *buffer = (char *)malloc(255 * sizeof(char));
  testFilePtr = fopen(pathToTestFile, "r");

  while (fgets(buffer, 255, (FILE *)testFilePtr) != NULL) {
    if (isTestCaseDefinition(buffer)) {
      addSingleTestCaseToList(testFileList, pathToTestFile, buffer);
    }
  }

  free(buffer);
  fclose(testFilePtr);
}

void addSingleTestCaseToList(TestFileList *testFileList,
                             const char *pathToTestFile, char *buffer) {
  TestFile *testFile = &testFileList->files[testFileList->size];
  testFile->cases = (TestCase *)realloc(
      testFile->cases, ((testFile->numTestCases + 1) * sizeof(TestCase)));
  testFile->cases[testFile->numTestCases].testName =
      (char *)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char *));

  trimTestName(buffer);

  strcpy(testFile->cases[testFile->numTestCases].testName, buffer);
  testFile->numTestCases++;
  testFileList->totalNumTestCases++;
}