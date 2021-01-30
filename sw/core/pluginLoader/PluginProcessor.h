#ifndef PLUGIN_PROCESSOR_H
#define PLUGIN_PROCESSOR_H

#include "PluginDefs.h"

#include "../common/BuildSequenceStep.h"
#include "../../external/GregCToolkit/sw/Collections/LinkedList.h"
#include <windows.h>

void processPlugins(LinkedList *buildSequence, PluginList *list,
                    LinkedList *pluginHModules, LinkedList *commandLineOptions);
void processBeforeAndAfterSteps(const HMODULE* hLib, LinkedList* buildSequence, LinkedList* commandLineOptions, const char* functionName, const char* beforeFunctionName,const char* afterFunctionName);
int indexOf(LinkedList* buildSequence, const char* functionName); 

#endif