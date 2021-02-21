#include "FileAndTestCaseGatherer.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions.h"
#include "../../external/GregCToolkit/sw/FailureHandling/FailureHandling.h"
#include "../../external/GregCToolkit/sw/FileIO/FileReader.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../common/MainFunctionFile.h"
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
      checkForMainFunction(fileOrSubDirectoryFullPath);
   }
   else if (isVisibleDirectory(fileOrSubDirectory))
   {
      loadTestsAndSourceFiles(testFiles, sourceFiles, tempObjectFiles, errorOnPreviousStep, fileOrSubDirectoryFullPath);
   }
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
      list->files[list->size].name = calloc(strlen(pathToSourceFile), sizeof(char));
      strcpy(list->files[list->size].name, pathToSourceFile);
      list->size++;
   }
}

void checkForMainFunction(const char* pathToSourceFile)
{
   ArgList* argList = malloc(sizeof(ArgList));
   argList->size = 1;
   argList->args = malloc(sizeof(void*));
   argList->args[0] = calloc(WINDOWS_MAX_PATH_LENGTH, sizeof(char));
   argList->args[0] = (void*)pathToSourceFile;
   readFileWithActionAfterEachLine(pathToSourceFile, argList, markMainFunctionLine);
}

int markMainFunctionLine(ArgList* argList)
{
   char filepath[MAX_LINE_LENGTH] = "";
   strcpy(filepath, (char*)argList->args[0]);

   char buffer[MAX_LINE_LENGTH] = "";
   strcpy(buffer, (char*)argList->args[argList->size - 1]);

   const char* mainFunctionVariants[48] = {
       "void main()",
       "void main(){",
       "void main(int argc, char* argv[])",
       "void main(int argc, char* argv[]){",
       "void main(int argc, char** argv)",
       "void main(int argc, char** argv){",
       "void main(int argc, const char* argv[])",
       "void main(int argc, const char* argv[]){",
       "void main(int argc, const char** argv)",
       "void main(int argc, const char** argv){",
       "void main(void)",
       "void main(void){"
       "int main()",
       "int main(){",
       "int main(int argc, char* argv[])",
       "int main(int argc, char* argv[]){",
       "int main(int argc, char** argv)",
       "int main(int argc, char** argv){",
       "int main(int argc, const char* argv[])",
       "int main(int argc, const char* argv[]){",
       "int main(int argc, const char** argv)",
       "int main(int argc, const char** argv){",
       "int main(void)",
       "int main(void){",
       "void main ()",
       "void main (){",
       "void main (int argc, char* argv[])",
       "void main (int argc, char* argv[]){",
       "void main (int argc, char** argv)",
       "void main (int argc, char** argv){",
       "void main (int argc, const char* argv[])",
       "void main (int argc, const char* argv[]){",
       "void main (int argc, const char** argv)",
       "void main (int argc, const char** argv){",
       "void main (void)",
       "void main (void){"
       "int main ()",
       "int main (){",
       "int main (int argc, char* argv[])",
       "int main (int argc, char* argv[]){",
       "int main (int argc, char** argv)",
       "int main (int argc, char** argv){",
       "int main (int argc, const char* argv[])",
       "int main (int argc, const char* argv[]){",
       "int main (int argc, const char** argv)",
       "int main (int argc, const char** argv){",
       "int main (void)",
       "int main (void){"};

   for (int i = 0; i < 46; i++)
   {
      const char* variant = mainFunctionVariants[i];
      if (strcmp(variant, buffer) == 0)
      {
         setMainFunctionFile(filepath);
      }
   }
   return 0;
}

void addTestFileToList(TestFileList* testFileList, const char* pathToTestFile)
{
   if (testFileList != NULL)
   {
      testFileList->files = (TestFile*)realloc(testFileList->files, (testFileList->size + 1) * sizeof(TestFile));
      testFileList->files[testFileList->size].name = calloc(strlen(pathToTestFile), sizeof(char));
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
   argsList->args = calloc(argsList->size, sizeof(char*));
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
   testFile->cases[testFile->numTestCases].testName = calloc(strlen(buffer), sizeof(char));

   strcpy(testFile->cases[testFile->numTestCases].testName, buffer);
   testFile->numTestCases++;
   testFileList->totalNumTestCases++;
}