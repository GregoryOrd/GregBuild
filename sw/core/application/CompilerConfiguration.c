#include "CompilerConfiguration.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/FileIO/FileReader.h"
#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/FileStructureDefs.h"
#include "../common/GregBuildConstants.h"
#include "CompilerConfigurationValues.h"

int readCompilerConfigurationFromFile()
{
   initOptionLists();

   ArgList* argList = malloc(sizeof(ArgList));
   argList->size = 0;
   argList->args = malloc(sizeof(void*));
   readFileWithActionAfterEachLine(COMPILER_CONFIG_FILE, argList, parseConfigurationFileLine);
   return 0;
}

int parseConfigurationFileLine(ArgList* argList)
{
   char buffer[MAX_LINE_LENGTH] = "";
   char param[WINDOWS_MAX_PATH_LENGTH] = "";
   char value[WINDOWS_MAX_PATH_LENGTH] = "";

   strcpy(buffer, (char*)argList->args[argList->size - 1]);
   parseParamAndValueFromBuffer(param, value, buffer);
   setConfigurations(param, value);

   return 0;
}

void parseParamAndValueFromBuffer(char* param, char* value, const char* buffer)
{
   bool delimiterReached = false;
   bool newLineReached = false;
   int indexOfDelimiter = 0;
   for (int i = 0; i < strlen(buffer); i++)
   {
      addCharToParamIfBeforeDelimiter(i, param, buffer, &delimiterReached, &indexOfDelimiter);
      addCharToValueIfAfterDelimiter(i, value, buffer, delimiterReached, indexOfDelimiter);
   }
}

void addCharToParamIfBeforeDelimiter(int i, char* param, const char* buffer, bool* delimiterReached, int* indexOfDelimiter)
{
   if (buffer[i] != COMPILER_CONFIG_DELIMITER && !(*delimiterReached))
   {
      param[i] = buffer[i];
   }
   else if (buffer[i] == COMPILER_CONFIG_DELIMITER)
   {
      *delimiterReached = true;
      *indexOfDelimiter = i;
      i++;
   }
}

void addCharToValueIfAfterDelimiter(int i, char* value, const char* buffer, bool delimiterReached, int indexOfDelimiter)
{
   if (delimiterReached)
   {
      value[i - indexOfDelimiter - 1] = buffer[i];
   }
}

// To simpilify this, implement a hash table from the param string
// to a SetConfiguration struct. Each SetConfiguration struct can hold
// the data and function pointers it needs for setting the configuration.
// Then loop through the SetConfiguration struct and execute the function pointer.
//
/*
typedef void (*SET_CONFIGURATION_ACTION)(void**)
typedef struct SetConfiguration
{
   SET_CONFIGURATION_ACTION[2] actions;
   void** dataToActOn;
} SetConfiguration

An example action could look like:

void thisAction(void** args)
{
   LinkedList* list = (LinkedList*)args[0];
   const char* value = (const char*)args[1];
   append_string_ll(list, value, COMPILER_OPTION_TYPE);
}
*/
void setConfigurations(const char* param, const char* value)
{
   if (strcmp(param, "host") == 0)
   {
      strcpy(hostCompiler_, value);
   }
   else if (strcmp(param, "target") == 0)
   {
      strcpy(targetCompiler_, value);
   }
   else if (strcmp(param, "compilerOption") == 0)
   {
      append_string_ll(hostCompilerOptions_, value, COMPILER_OPTION_TYPE);
      append_string_ll(targetCompilerOptions_, value, COMPILER_OPTION_TYPE);
   }
   else if (strcmp(param, "hostCompilerOption") == 0)
   {
      append_string_ll(hostCompilerOptions_, value, COMPILER_OPTION_TYPE);
   }
   else if (strcmp(param, "targetCompilerOption") == 0)
   {
      append_string_ll(targetCompilerOptions_, value, COMPILER_OPTION_TYPE);
   }
   else if (strcmp(param, "linkerOption") == 0)
   {
      append_string_ll(hostLinkerOptions_, value, LINKER_OPTION_TYPE);
      append_string_ll(targetLinkerOptions_, value, LINKER_OPTION_TYPE);
   }
   else if (strcmp(param, "hostLinkerOption") == 0)
   {
      append_string_ll(hostLinkerOptions_, value, LINKER_OPTION_TYPE);
   }
   else if (strcmp(param, "targetLinkerOption") == 0)
   {
      append_string_ll(targetLinkerOptions_, value, LINKER_OPTION_TYPE);
   }
}

void initOptionLists()
{
   hostCompilerOptions_ = malloc(sizeof(LinkedList));
   targetCompilerOptions_ = malloc(sizeof(LinkedList));
   initEmptyLinkedList(hostCompilerOptions_, COMPILER_OPTION_TYPE);
   initEmptyLinkedList(targetCompilerOptions_, COMPILER_OPTION_TYPE);

   hostLinkerOptions_ = malloc(sizeof(LinkedList));
   targetLinkerOptions_ = malloc(sizeof(LinkedList));
   initEmptyLinkedList(hostLinkerOptions_, LINKER_OPTION_TYPE);
   initEmptyLinkedList(targetLinkerOptions_, LINKER_OPTION_TYPE);
}

char* hostCompiler() { return hostCompiler_; }

char* targetCompiler() { return targetCompiler_; }

LinkedList* hostCompilerOptions()
{
   if (hostCompilerOptions_ == NULL)
   {
      hostCompilerOptions_ = malloc(sizeof(LinkedList));
      initEmptyLinkedList(hostCompilerOptions_, COMPILER_OPTION_TYPE);
   }
   return hostCompilerOptions_;
}

LinkedList* targetCompilerOptions()
{
   if (targetCompilerOptions_ == NULL)
   {
      targetCompilerOptions_ = malloc(sizeof(LinkedList));
      initEmptyLinkedList(targetCompilerOptions_, COMPILER_OPTION_TYPE);
   }
   return targetCompilerOptions_;
}

LinkedList* hostLinkerOptions()
{
   if (hostLinkerOptions_ == NULL)
   {
      hostLinkerOptions_ = malloc(sizeof(LinkedList));
      initEmptyLinkedList(hostLinkerOptions_, LINKER_OPTION_TYPE);
   }
   return hostLinkerOptions_;
}

LinkedList* targetLinkerOptions()
{
   if (targetLinkerOptions_ == NULL)
   {
      targetLinkerOptions_ = malloc(sizeof(LinkedList));
      initEmptyLinkedList(targetLinkerOptions_, LINKER_OPTION_TYPE);
   }
   return targetLinkerOptions_;
}