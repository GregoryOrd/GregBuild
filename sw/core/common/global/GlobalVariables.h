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

   bool testsWereRun();
   void setTestsWereRun();

   int readConfigurationsFromFile();

   char* hostCompiler();
   char* targetCompiler();

   LinkedList* hostCompilerOptions();
   LinkedList* targetCompilerOptions();
   LinkedList* hostLinkerOptions();
   LinkedList* targetLinkerOptions();

   LinkedList* hostExcludedFiles();

   void freeGlobalOptionsLists();
   void freeHostExcludedFilesList();

#ifdef __cplusplus
}
#endif

#endif