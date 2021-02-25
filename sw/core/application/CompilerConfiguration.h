#ifndef COMPILER_CONFIGURATION
#define COMPILER_CONFIGURATION

#include <stdbool.h>

#include "../../external/GregCToolkit/sw/Collections/LinkedList/LinkedList.h"
#include "../../external/GregCToolkit/sw/ExternalProgramExecution/ArgListDefs.h"

#define COMPILER_CONFIG_DELIMITER ':'

#ifdef __cplusplus
extern "C"
{
#endif

   int readCompilerConfigurationFromFile();
   int parseConfigurationFileLine(ArgList* argList);
   void parseParamAndValueFromBuffer(char* param, char* value, const char* buffer);
   void addCharToParamIfBeforeDelimiter(int i, char* param, const char* buffer, bool* delimiterReached, int* indexOfDelimiter);
   void addCharToValueIfAfterDelimiter(int i, char* value, const char* buffer, bool delimiterReached, int indexOfDelimiter);
   void setConfigurations(const char* param, const char* value);
   void initOptionLists();

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