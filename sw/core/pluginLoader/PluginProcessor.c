#include "PluginProcessor.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifdef __WINDOWS__
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions_ll.h"
#include "../common/BuildSequenceStep.h"
#include "../common/GregBuildConstants.h"

typedef BuildSequenceStep* (*PluginFunction)();

void processPlugins(LinkedList* buildSequence, const PluginList* list, LinkedList* pluginModules, LinkedList* commandLineOptions)
{
   int numCoreBuildSequenceSteps = buildSequence->size;
   BuildSequenceStep* coreBuildSequence = calloc(buildSequence->size, sizeof(BuildSequenceStep));
   storeCurrentBuildSequenceIntoCoreBuildSequenceList(coreBuildSequence, buildSequence);
   searchPluginsAndAddStepsToBuildSequence(coreBuildSequence, numCoreBuildSequenceSteps, buildSequence, list, pluginModules, commandLineOptions);
   freeCoreBuildSequenceList(coreBuildSequence, numCoreBuildSequenceSteps);
}

#ifdef __WINDOWS__
void processBeforeAndAfterSteps(
    const HMODULE* lib, LinkedList* buildSequence, LinkedList* commandLineOptions, const char* functionName, const char* beforeFunctionName, const char* afterFunctionName)
{
   PluginFunction beforeFunction = (PluginFunction)GetProcAddress(*lib, beforeFunctionName);
   int index = indexOf(buildSequence, functionName);
   if (beforeFunction != NULL && index != -1)
   {
      int beforeIndex = index;
      BuildSequenceStep* beforeStep = beforeFunction();
      insert_ll(buildSequence, beforeStep, BUILD_SEQUENCE_STEP_TYPE, beforeIndex);
      addOptionIfItDoesntAlreadyExist(commandLineOptions, beforeStep->option, COMMAND_LINE_OPTION_TYPE, beforeIndex);
   }

   PluginFunction afterFunction = (PluginFunction)GetProcAddress(*lib, afterFunctionName);
   index = indexOf(buildSequence, functionName);
   if (afterFunction != NULL && index != -1)
   {
      int afterIndex = index + 1;
      BuildSequenceStep* afterStep = afterFunction();
      insert_ll(buildSequence, afterStep, BUILD_SEQUENCE_STEP_TYPE, afterIndex);
      addOptionIfItDoesntAlreadyExist(commandLineOptions, afterStep->option, COMMAND_LINE_OPTION_TYPE, afterIndex);
   }
}
#else
void processBeforeAndAfterSteps(
    const void* lib, LinkedList* buildSequence, LinkedList* commandLineOptions, const char* functionName, const char* beforeFunctionName, const char* afterFunctionName)
{
   PluginFunction beforeFunction = (PluginFunction)dlsym((void*)lib, beforeFunctionName);
   int index = indexOf(buildSequence, functionName);
   if (beforeFunction != NULL && index != -1)
   {
      int beforeIndex = index;
      BuildSequenceStep* beforeStep = beforeFunction();
      insert_ll(buildSequence, beforeStep, BUILD_SEQUENCE_STEP_TYPE, beforeIndex);
      addOptionIfItDoesntAlreadyExist(commandLineOptions, beforeStep->option, COMMAND_LINE_OPTION_TYPE, beforeIndex);
   }

   PluginFunction afterFunction = (PluginFunction)dlsym((void*)lib, afterFunctionName);
   index = indexOf(buildSequence, functionName);
   if (afterFunction != NULL && index != -1)
   {
      int afterIndex = index + 1;
      BuildSequenceStep* afterStep = afterFunction();
      insert_ll(buildSequence, afterStep, BUILD_SEQUENCE_STEP_TYPE, afterIndex);
      addOptionIfItDoesntAlreadyExist(commandLineOptions, afterStep->option, COMMAND_LINE_OPTION_TYPE, afterIndex);
   }
}
#endif

int indexOf(const LinkedList* buildSequence, const char* functionName)
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

void storeCurrentBuildSequenceIntoCoreBuildSequenceList(BuildSequenceStep* coreBuildSequence, const LinkedList* buildSequence)
{
   int numCoreBuildSequenceSteps = buildSequence->size;
   for (int i = 0; i < numCoreBuildSequenceSteps; i++)
   {
      const BuildSequenceStep* step = (BuildSequenceStep*)at_ll(buildSequence, BUILD_SEQUENCE_STEP_TYPE, i);
      allocateAndCopyBuildSequenceStep(&coreBuildSequence[i], step);
   }
}

void freeCoreBuildSequenceList(BuildSequenceStep* coreBuildSequence, int numCoreBuildSequenceSteps)
{
   for (int i = 0; i < numCoreBuildSequenceSteps; i++)
   {
      free(coreBuildSequence[i].option->optionText);
      free(coreBuildSequence[i].option->description);
      free(coreBuildSequence[i].option);
      free(coreBuildSequence[i].functionName);
   }
   free(coreBuildSequence);
}

void searchPluginsAndAddStepsToBuildSequence(
    BuildSequenceStep* coreBuildSequence, int numCoreBuildSequenceSteps, LinkedList* buildSequence, const PluginList* list, LinkedList* pluginModules,
    LinkedList* commandLineOptions)
{
   for (int i = 0; i < list->size; i++)
   {
#ifdef __WINDOWS__
      const HMODULE* lib = (const HMODULE*)at_ll(pluginModules, PLUGIN_MODULE_LL_TYPE, i);
#else
      const void* lib = (const void*)at_ll(pluginModules, PLUGIN_MODULE_LL_TYPE, i);
#endif
      for (int j = 0; j < numCoreBuildSequenceSteps; j++)
      {
         char beforeFunctionName[WINDOWS_MAX_PATH_LENGTH] = "before_";
         char afterFunctionName[WINDOWS_MAX_PATH_LENGTH] = "after_";
         strcat(beforeFunctionName, coreBuildSequence[j].functionName);
         strcat(afterFunctionName, coreBuildSequence[j].functionName);
         processBeforeAndAfterSteps(lib, buildSequence, commandLineOptions, coreBuildSequence[j].functionName, beforeFunctionName, afterFunctionName);
      }
   }
}