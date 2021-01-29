#include "PluginProcessor.h"

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions_ll.h"
#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/GregBuildConstants.h"

// Remove when done debugging
#include <stdio.h>

typedef BuildSequenceStep* (*PluginFunction)();

void processPlugins(LinkedList* buildSequence, PluginList* list, LinkedList* pluginHModules, LinkedList* commandLineOptions)
{
   // Will need to free this at the end of the process plugins function
   int numCoreBuildSequenceSteps = buildSequence->size;
   BuildSequenceStep* coreBuildSequence = (BuildSequenceStep*)malloc(buildSequence->size * sizeof(BuildSequenceStep));
   for (int i = 0; i < numCoreBuildSequenceSteps; i++)
   {
      BuildSequenceStep* step = (BuildSequenceStep*)at_ll(buildSequence, BUILD_SEQUENCE_STEP_TYPE, i);
      coreBuildSequence[i].option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
      coreBuildSequence[i].option->optionText = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
      coreBuildSequence[i].option->description = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
      coreBuildSequence[i].option->flagValue = (bool*)malloc(sizeof(bool));
      coreBuildSequence[i].functionName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));

      strcpy(coreBuildSequence[i].option->description, step->option->description);
      strcpy(coreBuildSequence[i].option->optionText, step->option->optionText);
      coreBuildSequence[i].option->flagValue = step->option->flagValue;
      coreBuildSequence[i].function_ptr = step->function_ptr;
      strcpy(coreBuildSequence[i].functionName, step->functionName);
   }

   for (int i = 0; i < list->size; i++)
   {
      const HMODULE* hLib = (const HMODULE*)at_ll(pluginHModules, HMODULE_LL_TYPE, i);
      printf("Looking at plugin: %d\n", i);
      for (int j = 0; j < numCoreBuildSequenceSteps; j++)
      {
         char beforeFunctionName[WINDOWS_MAX_PATH_LENGTH];
         clearString(beforeFunctionName);
         char afterFunctionName[WINDOWS_MAX_PATH_LENGTH];
         clearString(afterFunctionName);
         strcat(beforeFunctionName, "before_");
         strcat(beforeFunctionName, coreBuildSequence[j].functionName);
         strcat(afterFunctionName, "after_");
         strcat(afterFunctionName, coreBuildSequence[j].functionName);
         printf("%s || %s || %s\n", coreBuildSequence[j].functionName, beforeFunctionName, afterFunctionName);
         processBeforeAndAfterSteps(hLib, buildSequence, commandLineOptions, coreBuildSequence[j].functionName, beforeFunctionName, afterFunctionName);
      }
   }

   for (int i = 0; i < numCoreBuildSequenceSteps; i++)
   {
      free(coreBuildSequence[i].option->optionText);
      free(coreBuildSequence[i].option->description);
      free(coreBuildSequence[i].option);
      free(coreBuildSequence[i].functionName);
   }
   free(coreBuildSequence);
}

void processBeforeAndAfterSteps(
    const HMODULE* hLib, LinkedList* buildSequence, LinkedList* commandLineOptions, const char* functionName, const char* beforeFunctionName, const char* afterFunctionName)
{
   PluginFunction beforeFunction = (PluginFunction)GetProcAddress(*hLib, beforeFunctionName);
   int index = indexOf(buildSequence, functionName);
   if (beforeFunction != NULL && index != -1)
   {
      int beforeIndex = index - 1;
      if (beforeIndex == -1)
      {
         beforeIndex = 0;
      }
      BuildSequenceStep* beforeStep = beforeFunction();
      insert_ll(buildSequence, beforeStep, BUILD_SEQUENCE_STEP_TYPE, beforeIndex);
      addOptionIfItDoesntAlreadyExist(commandLineOptions, beforeStep->option, COMMAND_LINE_OPTION_TYPE, beforeIndex);
   }

   PluginFunction afterFunction = (PluginFunction)GetProcAddress(*hLib, afterFunctionName);
   index = indexOf(buildSequence, functionName);
   if (afterFunction != NULL && index != -1)
   {
      int afterIndex = index + 1;
      BuildSequenceStep* afterStep = afterFunction();
      insert_ll(buildSequence, afterStep, BUILD_SEQUENCE_STEP_TYPE, afterIndex);
      addOptionIfItDoesntAlreadyExist(commandLineOptions, afterStep->option, COMMAND_LINE_OPTION_TYPE, afterIndex);
   }
   else if (afterFunction == NULL)
   {
      printf("Couldn't find afterFunction\n");
   }
}

int indexOf(LinkedList* buildSequence, const char* functionName)
{
   for (int i = 0; i < buildSequence->size; i++)
   {
      BuildSequenceStep* step = (BuildSequenceStep*)at_ll(buildSequence, BUILD_SEQUENCE_STEP_TYPE, i);
      if (step->functionName != NULL && strcmp(step->functionName, functionName) == 0)
      {
         return i;
      }
   }
   return -1;
}

void copyBuildSequence(BuildSequenceStep dest[], LinkedList* src) {}