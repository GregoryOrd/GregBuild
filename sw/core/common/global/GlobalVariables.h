#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <stdbool.h>

#include "../../../external/GregCToolkit/sw/Collections/LinkedList/LinkedList.h"

#ifdef __cplusplus
extern "C"
{
#endif

   bool hostCompileFailed();
   void setHostCompileFailed();

   bool testBuild();
   void setTestBuild(bool runTests);

   int readConfigurationsFromFile();

   const char* hostCompiler();
   const char* targetCompiler();
   const char* projectExecutableName();
   const char* buildStartingDirectory();
   const char* testFrameworkLibrary();

   LinkedList* hostCompilerOptions();
   LinkedList* targetCompilerOptions();
   LinkedList* hostLinkerOptions();
   LinkedList* targetLinkerOptions();

   LinkedList* hostExcludedFiles();
   LinkedList* targetExcludedFiles();

   LinkedList* hardwareSimulationLibraries();

   void freeGlobalOptionsLists();
   void freeHostExcludedFilesList();
   void freeTargetExcludedFilesList();
   void freeHardwareSimulationLibrariesList();

#ifdef __cplusplus
}
#endif

#endif