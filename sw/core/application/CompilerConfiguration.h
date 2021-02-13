#ifndef COMPILER_CONFIGURATION
#define COMPILER_CONFIGURATION

#include "../../external/GregCToolkit/sw/Collections/LinkedList.h"
#include "../../external/GregCToolkit/sw/ExternalProgramExecution/ArgListDefs.h"

#ifdef __cplusplus
extern "C"
{
#endif

   int readCompilerConfigurationFromFile();
   int parseConfigurationFileLine(ArgList* argList);
   char* hostCompiler();
   char* targetCompiler();

   LinkedList* compilerOptions();
   LinkedList* linkerOptions();

#ifdef __cplusplus
}
#endif

#endif