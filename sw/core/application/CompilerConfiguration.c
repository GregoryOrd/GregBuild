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
   ArgList* argList = malloc(sizeof(ArgList));
   argList->size = 0;
   argList->args = malloc(sizeof(void*));
   readFileWithActionAfterEachLine(COMPILER_CONFIG_FILE, argList, parseConfigurationFileLine);
   return 0;
}

int parseConfigurationFileLine(ArgList* argList)
{
   char* buffer = (char*)argList->args[argList->size - 1];

   bool equalSignReached = false;
   bool newLineReached = false;

   char param[WINDOWS_MAX_PATH_LENGTH] = "";
   char value[WINDOWS_MAX_PATH_LENGTH] = "";
   int indexOfEqualSign = 0;
   for (int i = 0; i < strlen(buffer); i++)
   {
      if (buffer[i] != '=' && !equalSignReached)
      {
         param[i] = buffer[i];
      }
      else if (buffer[i] == '=')
      {
         equalSignReached = true;
         indexOfEqualSign = i;
         i++;
      }

      if (equalSignReached)
      {
         value[i - indexOfEqualSign - 1] = buffer[i];
      }
   }

   if (strcmp(param, "host") == 0)
   {
      strcpy(hostCompiler_, value);
      printf("Host Compiler: %s\n", hostCompiler_);
   }
   else if (strcmp(param, "target") == 0)
   {
      strcpy(targetCompiler_, value);
      printf("Target Compiler: %s\n", targetCompiler_);
   }
   return 0;
}

char* hostCompiler()
{
   printf("Host Compiler: %s\n", hostCompiler_);
   return hostCompiler_;
}

char* targetCompiler()
{
   printf("Target Compiler: %s\n", targetCompiler_);
   return targetCompiler_;
}