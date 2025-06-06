#include "CompileAndLinkHelpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h> 
#include <unistd.h>

#include "../../external/GregCToolkit/sw/ExternalProgramExecution/CommandLineExecutables.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/GregBuildConstants.h"
#include "../common/ObjAndTempDefs.h"
#include "../common/TestAndSrcDefinitions.h"
#include "../common/global/GlobalVariables.h"

void addTempObjectFileToList(ObjectFileList* list, const char* filename, char* tempObjectFile, const char* compiler)
{
   tempDirPathFromCompiler(tempObjectFile, compiler);
   strcat(tempObjectFile, DELIMITER);
   strcat(tempObjectFile, filename);

   list->files = (ObjectFile*)realloc(list->files, ((list->size + 1) * sizeof(ObjectFile)));
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

void initArgsForLinkingProjectExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles, const char* compiler)
{
   LinkedList* options = determineLinkerOptionsListFromCompiler(compiler);
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

   char distDirectoryExecutableName[WINDOWS_MAX_PATH_LENGTH] = DIST;
   strcat(distDirectoryExecutableName, "/");
   strcat(distDirectoryExecutableName, (char*)projectExecutableName());
   strcpy(linkerArgs->args[linkerArgs->size - 3], "-o");
   strcpy(linkerArgs->args[linkerArgs->size - 2], distDirectoryExecutableName);
   linkerArgs->args[linkerArgs->size - 1], NULL;
}

void fileArgsForLinkingProjectExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles)
{
   int numObjectFilesFromSourceAddedToArgsList = 0;
   LinkedList* options = targetLinkerOptions();

   for (int i = 0; i < tempObjectFiles->size; i++)
   {
      ObjectFile* file = &tempObjectFiles->files[i];
      if (file->isFromSource)
      {
         strcpy(linkerArgs->args[numObjectFilesFromSourceAddedToArgsList + options->size + 1], file->name);
         numObjectFilesFromSourceAddedToArgsList++;
      }
   }
}

void initArgsForLinkingTestExecutable(ArgList* linkerArgs, const ObjectFileList* tempObjectFiles, const char* compiler)
{
   LinkedList* options = determineLinkerOptionsListFromCompiler(compiler);
   // The +7 is for the known args below
   linkerArgs->size = tempObjectFiles->size - 1 + options->size + 7;
   linkerArgs->args = calloc(linkerArgs->size, sizeof(void*));
   for (int i = 0; i < linkerArgs->size - 1; i++)
   {
      linkerArgs->args[i] = calloc(WINDOWS_MAX_PATH_LENGTH, sizeof(char));
   }

   strcpy(linkerArgs->args[0], compiler);
   strcpy(linkerArgs->args[1], "-shared");
   strcpy(linkerArgs->args[2], "-o");
   strcpy(linkerArgs->args[3], TEMP_TEST_PROJECT_LIBRARY);

   for (int j = 0; j < options->size - 1; j++)
   {
      strcpy(linkerArgs->args[j + 4], (char*)at_ll(options, LINKER_OPTION_TYPE, j));
   }

   char testFrameworkPath[WINDOWS_MAX_PATH_LENGTH];
   strcpy(testFrameworkPath, testFrameworkLibrary());
   char *filename = strdup(testFrameworkPath);  
   char *base = basename(filename);

   char *dot = strrchr(base, '.');
   if (dot != NULL) {
       *dot = '\0'; 
   }

   char testFrameworkLinkerArg[WINDOWS_MAX_PATH_LENGTH];
   snprintf(testFrameworkLinkerArg, sizeof(testFrameworkLinkerArg), "-l%s", base + 3);

   char cwd[WINDOWS_MAX_PATH_LENGTH];
   getcwd(cwd, sizeof(cwd));
   char tempLibraryPath[WINDOWS_MAX_PATH_LENGTH + 7];
   snprintf(tempLibraryPath, sizeof(tempLibraryPath), "-L%s/temp", cwd);

   strcpy(linkerArgs->args[linkerArgs->size - 3], tempLibraryPath);
   strcpy(linkerArgs->args[linkerArgs->size - 2], testFrameworkLinkerArg);
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

void initArgsForCreatingTestMainExecutable(ArgList* linkerArgs)
{
   int numConcreteArgs = 8;
   LinkedList* hardwareSimLibraries = hardwareSimulationLibraries();
   linkerArgs->size = hardwareSimLibraries->size + numConcreteArgs;
   linkerArgs->args = calloc(linkerArgs->size, sizeof(void*));
   for (int i = 0; i < linkerArgs->size - 1; i++)
   {
      linkerArgs->args[i] = calloc(WINDOWS_MAX_PATH_LENGTH, sizeof(char));
   }

   char cwd[WINDOWS_MAX_PATH_LENGTH];
   getcwd(cwd, sizeof(cwd));
   char tempLibraryPath[WINDOWS_MAX_PATH_LENGTH + 7];
   snprintf(tempLibraryPath, sizeof(tempLibraryPath), "-L%s/temp", cwd);

   strcpy(linkerArgs->args[0], (char*)hostCompiler());
   strcpy(linkerArgs->args[1], "-o");
   strcpy(linkerArgs->args[2], TEMP_TEST_MAIN);
   strcpy(linkerArgs->args[3], TEMP_TEST_MAIN_C);
   strcpy(linkerArgs->args[4], tempLibraryPath);
   strcpy(linkerArgs->args[5], TEMP_TEST_PROJECT_LIBRARYLINKER_ARG);
   strcpy(linkerArgs->args[6], (char*)testFrameworkLibrary());

   for (int i = 0; i < hardwareSimLibraries->size; i++)
   {
      strcpy(linkerArgs->args[numConcreteArgs - 1 + i], at_ll(hardwareSimLibraries, HARDWARE_SIMULATION_LIBRARY, i));
   }
   linkerArgs->args[linkerArgs->size - 1] = NULL;
}

LinkedList* determineCompilerOptionsListFromCompiler(const char* compiler)
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

LinkedList* determineLinkerOptionsListFromCompiler(const char* compiler)
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
   return options;
}

void argsForCompilingToObjectFile(ArgList* compilerArgs, const char* filename, const char* tempObjectFileName, const char* compiler)
{
   LinkedList* options = determineCompilerOptionsListFromCompiler(compiler);
   compilerArgs->size = options->size + 6;
   compilerArgs->args = calloc(compilerArgs->size, sizeof(void*));
   for (int i = 0; i < compilerArgs->size - 1; i++)
   {
      compilerArgs->args[i] = calloc(WINDOWS_MAX_PATH_LENGTH, sizeof(char));
   }
   strcpy(compilerArgs->args[0], compiler);
   strcpy(compilerArgs->args[1], "-o");
   strcpy(compilerArgs->args[2], tempObjectFileName);
   strcpy(compilerArgs->args[3], "-c");

   for (int j = 0; j < options->size; j++)
   {
      strcpy(compilerArgs->args[j + 4], (char*)at_ll(options, COMPILER_OPTION_TYPE, j));
   }

   strcpy(compilerArgs->args[compilerArgs->size - 2], filename);
   compilerArgs->args[compilerArgs->size - 1] = NULL;
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