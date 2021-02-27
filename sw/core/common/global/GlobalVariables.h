#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <stdbool.h>

#include "../../../external/GregCToolkit/sw/Collections/LinkedList/LinkedList.h"

#ifdef __cplusplus
extern "C"
{
#endif

   bool testsWereRun();
   void setTestsWereRun();

   int readCompilerConfigurationFromFile();

   char* hostCompiler();
   char* targetCompiler();

   LinkedList* hostCompilerOptions();
   LinkedList* targetCompilerOptions();
   LinkedList* hostLinkerOptions();
   LinkedList* targetLinkerOptions();

#ifdef __cplusplus
}
#endif

#endif