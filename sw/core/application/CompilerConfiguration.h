#ifndef COMPILER_CONFIGURATION
#define COMPILER_CONFIGURATION

#include <stdbool.h>

#include "../../external/GregCToolkit/sw/ArgList/ArgList.h"
#include "../../external/GregCToolkit/sw/Collections/HashTable/HashTable.h"
#include "../../external/GregCToolkit/sw/Collections/LinkedList/LinkedList.h"

#define COMPILER_CONFIG_DELIMITER ':'

#ifdef __cplusplus
extern "C"
{
#endif

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