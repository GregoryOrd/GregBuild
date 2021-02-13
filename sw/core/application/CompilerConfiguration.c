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
   compilerOptions_ = malloc(sizeof(LinkedList));
   initEmptyLinkedList(compilerOptions_, COMPILER_OPTION_TYPE);
   linkerOptions_ = malloc(sizeof(LinkedList));
   initEmptyLinkedList(linkerOptions_, LINKER_OPTION_TYPE);

   ArgList* argList = malloc(sizeof(ArgList));
   argList->size = 0;
   argList->args = malloc(sizeof(void*));
   readFileWithActionAfterEachLine(COMPILER_CONFIG_FILE, argList, parseConfigurationFileLine);
   return 0;
}

int parseConfigurationFileLine(ArgList* argList)
{
   char buffer[MAX_LINE_LENGTH] = "";
   strcpy(buffer, (char*)argList->args[argList->size - 1]);

   bool colonReached = false;
   bool newLineReached = false;
   char delimiter = ':';

   char param[WINDOWS_MAX_PATH_LENGTH] = "";
   char value[WINDOWS_MAX_PATH_LENGTH] = "";
   int indexOfEqualSign = 0;
   for (int i = 0; i < strlen(buffer); i++)
   {
      if (buffer[i] != delimiter && !colonReached)
      {
         param[i] = buffer[i];
      }
      else if (buffer[i] == delimiter)
      {
         colonReached = true;
         indexOfEqualSign = i;
         i++;
      }

      if (colonReached)
      {
         value[i - indexOfEqualSign - 1] = buffer[i];
      }
   }

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
      append_string_ll(compilerOptions_, value, COMPILER_OPTION_TYPE);
   }
   else if (strcmp(param, "linkerOption") == 0)
   {
      append_string_ll(linkerOptions_, value, LINKER_OPTION_TYPE);
   }
   return 0;
}

char* hostCompiler() { return hostCompiler_; }

char* targetCompiler() { return targetCompiler_; }

LinkedList* compilerOptions()
{
   if (compilerOptions_ == NULL)
   {
      compilerOptions_ = malloc(sizeof(LinkedList));
      initEmptyLinkedList(compilerOptions_, COMPILER_OPTION_TYPE);
   }
   return compilerOptions_;
}

LinkedList* linkerOptions()
{
   if (linkerOptions_ == NULL)
   {
      linkerOptions_ = malloc(sizeof(LinkedList));
      initEmptyLinkedList(linkerOptions_, LINKER_OPTION_TYPE);
   }
   return linkerOptions_;
}