#include "FileOperations.h"

#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../common/GregBuildConstants.h"

void initFileListsAndTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles)
{
   initTestFiles(testFiles);
   initSourceFiles(sourceFiles);
   initObjectFileList(tempObjectFiles);
   makeDir(TEMP_DIR);
}

void initTestFiles(TestFileList* testFiles)
{
   if (testFiles != NULL)
   {
      testFiles->size = 0;
      testFiles->files = malloc(sizeof(TestFile));
      strcpy(testFiles->files[0].name, "");
      testFiles->files[0].numTestCases = 0;
      testFiles->totalNumTestCases = 0;
   }
}

void initSourceFiles(SourceFileList* sourceFiles)
{
   sourceFiles->size = 0;
   sourceFiles->files = malloc(sizeof(SourceFile));
   strcpy(sourceFiles->files[0].name, "");
}

void initObjectFileList(ObjectFileList* objectFiles)
{
   objectFiles->size = 0;
   objectFiles->files = malloc(sizeof(ObjectFile));
   strcpy(objectFiles->files[0].name, "");
}

void freeObjectFileList(ObjectFileList* list)
{
   free(list->files);
   free(list);
}

void resetObjectFileList(ObjectFileList* list)
{
   for (int i = 0; i < list->size; i++)
   {
      strcpy(list->files[0].name, "");
      list->files[i].isFromSource = false;
   }
   list->size = 0;
}

void freeFileLists(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles)
{
   if (testFiles != NULL)
   {
      freeTestFileList(testFiles);
   }
   freeSourceFileList(sourceFiles);
   freeObjectFileList(tempObjectFiles);
}

void freeSourceFileList(SourceFileList* list)
{
   free(list->files);
   free(list);
}

void freeTestFileList(TestFileList* testFileList)
{
   for (int fileIndex = 0; fileIndex < testFileList->size; fileIndex++)
   {
      TestFile* testFile = &testFileList->files[fileIndex];
      free(testFile->cases);
   }
   free(testFileList->files);
   free(testFileList);
}

int removeTempDir(const TestFileList* const testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   return removeDir(TEMP_DIR);
}