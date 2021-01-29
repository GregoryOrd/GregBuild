#include "PluginProcessor.h"

#include "../common/BuildSequenceStep.h"
#include "../common/GregBuildConstants.h"

typedef BuildSequenceStep* (*PluginFunction)();

void processPlugins(LinkedList* buildSequence, PluginList* list, LinkedList* pluginHModules, LinkedList* commandLineOptions)
{
   for (int i = 0; i < list->size; i++)
   {
      const HMODULE* hLib = (const HMODULE*)at_ll(pluginHModules, HMODULE_LL_TYPE, i);
      processBeforeAndAfterLoadingTestAndSourceFiles(hLib, buildSequence, commandLineOptions);

      // PluginFunction beforeCompileIntoTempObjectFiles = (PluginFunction)GetProcAddress(*hLib, "beforeCompileIntoTempObjectFiles");
      // PluginFunction afterCompileIntoTempObjectFiles = (PluginFunction)GetProcAddress(*hLib, "afterCompileIntoTempObjectFiles");

      // PluginFunction beforeLinkObjectFilesWithGregTestDllToMakeProjectTestDll =
      //     (PluginFunction)GetProcAddress(*hLib, "beforeLinkObjectFilesWithGregTestDllToMakeProjectTestDll");
      // PluginFunction beforeLoadingTestAndSourceFiles =
      //     (afterLinkObjectFilesWithGregTestDllToMakeProjectTestDll)GetProcAddress(*hLib, "afterLinkObjectFilesWithGregTestDllToMakeProjectTestDll");

      // PluginFunction beforeWriteTestsToTestMain = (PluginFunction)GetProcAddress(*hLib, "beforeWriteTestsToTestMain");
      // PluginFunction afterWriteTestsToTestMain = (PluginFunction)GetProcAddress(*hLib, "afterWriteTestsToTestMain");

      // PluginFunction beforeCreateTestMainExecutableFromProjectDllAndGregTestDll =
      //     (PluginFunction)GetProcAddress(*hLib, "beforeCreateTestMainExecutableFromProjectDllAndGregTestDll");
      // PluginFunction afterCreateTestMainExecutableFromProjectDllAndGregTestDll =
      //     (PluginFunction)GetProcAddress(*hLib, "afterCreateTestMainExecutableFromProjectDllAndGregTestDll");

      // PluginFunction beforeRunTestsWithExitStatusCheck = (PluginFunction)GetProcAddress(*hLib, "beforeRunTestsWithExitStatusCheck");
      // PluginFunction afterRunTestsWithExitStatusCheck = (PluginFunction)GetProcAddress(*hLib, "afterRunTestsWithExitStatusCheck");

      // PluginFunction beforeCompileObjectFilesIntoProjectExecutable = (PluginFunction)GetProcAddress(*hLib, "beforeCompileObjectFilesIntoProjectExecutable");
      // PluginFunction afterCompileObjectFilesIntoProjectExecutable = (PluginFunction)GetProcAddress(*hLib, "afterCompileObjectFilesIntoProjectExecutable");

      // PluginFunction beforeRemoveTempDir = (PluginFunction)GetProcAddress(*hLib, "beforeRemoveTempDir");
      // PluginFunction afterRemoveTempDir = (PluginFunction)GetProcAddress(*hLib, "afterRemoveTempDir");
   }
}

void processBeforeAndAfterLoadingTestAndSourceFiles(const HMODULE* hLib, LinkedList* buildSequence, LinkedList* commandLineOptions)
{
   PluginFunction beforeLoadTestsAndSourceFiles = (PluginFunction)GetProcAddress(*hLib, "beforeLoadTestsAndSourceFiles");
   int index = indexOf_loadTestsAndSourceFiles(buildSequence);
   if (beforeLoadTestsAndSourceFiles != NULL && index != -1)
   {
      int beforeIndex = index - 1;
      if (beforeIndex == -1)
      {
         beforeIndex = 0;
      }
      BuildSequenceStep* beforeLoadTestsAndSourceFilesStep = beforeLoadTestsAndSourceFiles();
      insert_ll(buildSequence, beforeLoadTestsAndSourceFilesStep, BUILD_SEQUENCE_STEP_TYPE, beforeIndex);
      addOptionIfItDoesntAlreadyExist(commandLineOptions, beforeLoadTestsAndSourceFilesStep->option, COMMAND_LINE_OPTION_TYPE, beforeIndex);
   }

   PluginFunction afterLoadTestsAndSourceFiles = (PluginFunction)GetProcAddress(*hLib, "afterLoadTestsAndSourceFiles");
   index = indexOf_loadTestsAndSourceFiles(buildSequence);
   if (afterLoadTestsAndSourceFiles != NULL && index != -1)
   {
      int afterIndex = index + 1;
      BuildSequenceStep* afterLoadTestsAndSourceFilesStep = afterLoadTestsAndSourceFiles();
      insert_ll(buildSequence, afterLoadTestsAndSourceFilesStep, BUILD_SEQUENCE_STEP_TYPE, afterIndex);
      addOptionIfItDoesntAlreadyExist(commandLineOptions, afterLoadTestsAndSourceFilesStep->option, COMMAND_LINE_OPTION_TYPE, afterIndex);
   }
}

int indexOf_loadTestsAndSourceFiles(LinkedList* buildSequence) { return indexOf(buildSequence, "loadTestsAndSourceFiles"); }
int indexOf_compileIntoTempObjectFiles(LinkedList* buildSequence) { return indexOf(buildSequence, "compileIntoTempObjectFiles"); }
int indexOf_linkObjectFilesWithGregTestDllToMakeProjectTestDll(LinkedList* buildSequence)
{
   return indexOf(buildSequence, "linkObjectFilesWithGregTestDllToMakeProjectTestDll");
}
int indexOf_writeTestsToTestMain(LinkedList* buildSequence) { return indexOf(buildSequence, "writeTestsToTestMain"); }
int indexOf_createTestMainExecutableFromProjectDllAndGregTestDll(LinkedList* buildSequence)
{
   return indexOf(buildSequence, "createTestMainExecutableFromProjectDllAndGregTestDll");
}
int indexOf_runTestsWithExitStatusCheck(LinkedList* buildSequence) { return indexOf(buildSequence, "runTestsWithExitStatusCheck"); }
int indexOf_compileObjectFilesIntoProjectExecutable(LinkedList* buildSequence) { return indexOf(buildSequence, "compileObjectFilesIntoProjectExecutable"); }
int indexOf_removeTempDir(LinkedList* buildSequence) { return indexOf(buildSequence, "indexOf_removeTempDir"); }

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