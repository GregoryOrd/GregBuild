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

   LinkedList* hostCompilerOptions();
   LinkedList* targetCompilerOptions();
   LinkedList* hostLinkerOptions();
   LinkedList* targetLinkerOptions();

   LinkedList* hostExcludedFiles();
   LinkedList* targetExcludedFiles();

   void freeGlobalOptionsLists();
   void freeHostExcludedFilesList();
   void freeTargetExcludedFilesList();

#ifdef __cplusplus
}
#endif

#endif