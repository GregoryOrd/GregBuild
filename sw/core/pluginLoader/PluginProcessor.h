#ifndef PLUGIN_PROCESSOR_H
#define PLUGIN_PROCESSOR_H

#include "PluginDefs.h"

#include "../common/BuildSequenceStep.h"
#include "../../external/GregCToolkit/sw/Collections/LinkedList.h"

#ifdef __cplusplus
extern "C" {
#endif  

#ifdef __WINDOWS__
#include <windows.h>
void processBeforeAndAfterSteps(const HMODULE* lib, LinkedList* buildSequence, LinkedList* commandLineOptions, const char* functionName, const char* beforeFunctionName, const char* afterFunctionName);
#else
void processBeforeAndAfterSteps(const void* lib, LinkedList* buildSequence, LinkedList* commandLineOptions, const char* functionName, const char* beforeFunctionName, const char* afterFunctionName);
#endif

void processPlugins(LinkedList *buildSequence, const PluginList *list,
                    LinkedList *pluginModules, LinkedList *commandLineOptions);
int indexOf(const LinkedList* buildSequence, const char* functionName); 
void storeCurrentBuildSequenceIntoCoreBuildSequenceList(BuildSequenceStep* coreBuildSequence, const LinkedList* buildSequence);
void freeCoreBuildSequenceList(BuildSequenceStep* coreBuildSequence, int numCoreBuildSequenceSteps);
void searchPluginsAndAddStepsToBuildSequence(
    BuildSequenceStep* coreBuildSequence, int numCoreBuildSequenceSteps, LinkedList* buildSequence, const PluginList* list, LinkedList* pluginModules,
    LinkedList* commandLineOptions);

#ifdef __cplusplus
}
#endif  

#endif