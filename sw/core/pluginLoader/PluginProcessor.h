#ifndef PLUGIN_PROCESSOR_H
#define PLUGIN_PROCESSOR_H

#include "PluginDefs.h"

#include "../../external/GregCToolkit/sw/Collections/LinkedList.h"
#include <windows.h>

typedef int (*StepIndexSearchFunction)(LinkedList*);

void processPlugins(LinkedList *buildSequence, PluginList *list,
                    LinkedList *pluginHModules, LinkedList *commandLineOptions);

void processBeforeAndAfterLoadingTestAndSourceFiles(const HMODULE* hLib, LinkedList* buildSequence, LinkedList* commandLineOptions);
void processBeforeAndAfterSteps(const HMODULE* hLib, LinkedList* buildSequence, LinkedList* commandLineOptions, StepIndexSearchFunction searchFunction, const char* beforeFunctionName,const char* afterFunctionName);

int indexOf_loadTestsAndSourceFiles(LinkedList* buildSequence);
int indexOf_compileIntoTempObjectFiles(LinkedList* buildSequence);
int indexOf_linkObjectFilesWithGregTestDllToMakeProjectTestDll(LinkedList* buildSequence);
int indexOf_writeTestsToTestMain(LinkedList* buildSequence);
int indexOf_createTestMainExecutableFromProjectDllAndGregTestDll(LinkedList* buildSequence);
int indexOf_runTestsWithExitStatusCheck(LinkedList* buildSequence);
int indexOf_compileObjectFilesIntoProjectExecutable(LinkedList* buildSequence);
int indexOf_removeTempDir(LinkedList* buildSequence);
int indexOf(LinkedList* buildSequence, const char* functionName); 

#endif