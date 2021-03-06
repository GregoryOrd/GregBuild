#include "CompileAndLinkHelpers.h"

#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/ExternalProgramExecution/CommandLineExecutables.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/GregBuildConstants.h"
#include "../common/TestAndSrcDefinitions.h"
#include "../common/global/GlobalVariables.h"

void addTempObjectFileToList(ObjectFileList* list, const char* filename, const char* compiler)
{
   char tempObjectFile[WINDOWS_MAX_PATH_LENGTH] = "";
   tempDirPathFromCompiler(tempObjectFile, compiler);
   strcat(tempObjectFile, DELIMITER);
   strcat(tempObjectFile, filename);

   list->files = (ObjectFile*)realloc(list->files, ((list->size + 1) * sizeof(ObjectFile)));
   list->files[list->size].name = calloc(WINDOWS_MAX_PATH_LENGTH, sizeof(char));
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

int testFilesSize(const TestFileList* testFiles)
{
   int testFilesSize = 0;
   if (testFiles != NULL)
   {
      testFilesSize = testFiles->size;
   }
   return testFilesSize;
}

void initArgsForLinkingProjectExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles, char* compiler)
{
   LinkedList* options;
   if (stringsAreEqual(compiler, hostCompiler()))
   {
      options = hostLinkerOptions();
   }
   else
   {
      options = targetLinkerOptions();
   }
   linkerArgs->size = numObjectFilesFromSource(tempObjectFiles) + options->size + 4;
   linkerArgs->args = calloc(linkerArgs->size, sizeof(void*));
   for (int i = 0; i < linkerArgs->size; i++)
   {
      linkerArgs->args[i] = calloc(WINDOWS_MAX_PATH_LENGTH, sizeof(char));
   }
   strcpy(linkerArgs->args[0], compiler);

   for (int j = 0; j < options->size; j++)
   {
      strcpy(linkerArgs->args[j + 1], (char*)at_ll(options, LINKER_OPTION_TYPE, j));
   }

   strcpy(linkerArgs->args[linkerArgs->size - 3], "-o");
   strcpy(linkerArgs->args[linkerArgs->size - 2], PROJECT_EXE);
   linkerArgs->args[linkerArgs->size - 1], NULL;
}

void fileArgsForLinkingProjectExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles)
{
   int numObjectFilesFromSourceAddedToArgsList = 0;
   for (int i = 0; i < tempObjectFiles->size; i++)
   {
      ObjectFile* file = &tempObjectFiles->files[i];
      if (file->isFromSource)
      {
         strcpy(linkerArgs->args[numObjectFilesFromSourceAddedToArgsList + 1], file->name);
         numObjectFilesFromSourceAddedToArgsList++;
      }
   }
}

void initArgsForLinkingTestExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles, char* compiler)
{
   LinkedList* options;
   if (stringsAreEqual(compiler, hostCompiler()))
   {
      options = hostLinkerOptions();
   }
   else
   {
      options = targetLinkerOptions();
   }
   // The +7 is for the known args below
   // The -1 is to not include the main function .o file
   linkerArgs->size = tempObjectFiles->size + options->size + 7 - 1;
   linkerArgs->args = calloc(linkerArgs->size, sizeof(void*));
   for (int i = 0; i < linkerArgs->size - 1; i++)
   {
      linkerArgs->args[i] = calloc(WINDOWS_MAX_PATH_LENGTH, sizeof(char));
   }

   strcpy(linkerArgs->args[0], compiler);
   strcpy(linkerArgs->args[1], "-shared");
   strcpy(linkerArgs->args[2], "-o");
   strcpy(linkerArgs->args[3], TEMP_TEST_PROJECT_LIBRARY);

   for (int j = 0; j < options->size; j++)
   {
      strcpy(linkerArgs->args[j + 4], (char*)at_ll(options, LINKER_OPTION_TYPE, j));
   }

   strcpy(linkerArgs->args[linkerArgs->size - 3], "-L./");
   strcpy(linkerArgs->args[linkerArgs->size - 2], LIB_GREG_TEST_LIBRARY);
   linkerArgs->args[linkerArgs->size - 1] = NULL;
}

void fileArgsForLinkingTestExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles)
{
   int offset = 4;
   bool atLeastOneFileAdded = false;
   for (int i = 0; i < tempObjectFiles->size; i++)
   {
      if (i < tempObjectFiles->size)
      {
         strcpy(linkerArgs->args[i + offset], (&tempObjectFiles->files[i])->name);
      }
   }
}

void initArgsForCompilingToObjectFiles(ArgList* compilerArgs, const SourceFileList* sourceFiles, int numTestFiles, char* compiler)
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
   compilerArgs->size = numTestFiles + sourceFiles->size + options->size + 3;
   compilerArgs->args = calloc(compilerArgs->size, sizeof(void*));
   for (int i = 0; i < compilerArgs->size - 1; i++)
   {
      compilerArgs->args[i] = calloc(WINDOWS_MAX_PATH_LENGTH, sizeof(char));
   }
   strcpy(compilerArgs->args[0], compiler);
   strcpy(compilerArgs->args[1], "-c");

   for (int j = 0; j < options->size; j++)
   {
      strcpy(compilerArgs->args[j + 2], (char*)at_ll(options, COMPILER_OPTION_TYPE, j));
   }

   compilerArgs->args[compilerArgs->size - 1] = NULL;
}

void initMvArgsForMovingCompiledObjectFilesToTempDir(ArgList* mvArgs, const SourceFileList* sourceFiles, int numTestFiles, char* compiler)
{
   mvArgs->size = numTestFiles + sourceFiles->size + 3;
   mvArgs->args = calloc(mvArgs->size, sizeof(void*));

   for (int i = 0; i < mvArgs->size - 1; i++)
   {
      mvArgs->args[i] = calloc(WINDOWS_MAX_PATH_LENGTH, sizeof(char));
   }

   strcpy(mvArgs->args[0], mv);
   char tempFilePath[WINDOWS_MAX_PATH_LENGTH] = "";
   tempDirPathFromCompiler(tempFilePath, compiler);
   makeDir(tempFilePath);
   strcpy(mvArgs->args[mvArgs->size - 2], tempFilePath);
   mvArgs->args[mvArgs->size - 1] = NULL;
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

void copyTempObjectOrCFileNameIntoArgList(ArgList* argList, int* argIndex, int offset, int optionsOffset, const void* fileList, int index, char* objectFileName, int listType)
{
   if (offset == mvFileArgOffset)
   {
      strcpy(argList->args[*argIndex + offset + optionsOffset], objectFileName);
   }
   else if (offset == gccFileArgOffset)
   {
      strCopyUsingListType(listType, argList->args[*argIndex + offset + optionsOffset], fileList, index);
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