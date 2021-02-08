#include "../../external/GregCToolkit/sw/Collections/LinkedList.h"
#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions_ll.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../application/CompileAndLinkCommands.h"
#include "../application/CoreCommandLineOptions.h"
#include "../application/RunTests.h"
#include "../common/FileStructureDefs.h"
#include "../common/GregBuildConstants.h"
#include "../fileSystemRecursion/FileAndTestCaseGatherer.h"
#include "../fileSystemRecursion/FileOperations.h"
#include "../pluginLoader/PluginLoader.h"
#include "../pluginLoader/PluginProcessor.h"
#include "../testMainWriting/TestMainWriter.h"
#include "BuildSequence.h"

int main(int argc, const char* argv[])
{
   printf("Starting GregBuild\n");
   LinkedList* optionList = malloc(sizeof(LinkedList));
   TestFileList* testFiles;
   SourceFileList* sourceFiles = malloc(sizeof(SourceFileList));
   ObjectFileList* tempObjectFiles = malloc(sizeof(ObjectFileList));
   PluginList* plugins = malloc(sizeof(PluginList));
   LinkedList* pluginHModules = malloc(sizeof(LinkedList));

   initCoreCommandLineOptions(optionList);
   if (flagValueForOption_ll(optionList, NO_TEST_OPTION_TEXT, COMMAND_LINE_OPTION_TYPE))
   {
      testFiles = malloc(sizeof(TestFileList));
   }
   initFileListsAndTempDir(testFiles, sourceFiles, tempObjectFiles);

   LinkedList* buildSequence = malloc(sizeof(LinkedList));
   initBuildSequence(buildSequence);

   initEmptyLinkedList(pluginHModules, HMODULE_LL_TYPE);
   initPluginList(plugins);
   loadPlugins(plugins, pluginHModules, PLUGINS_LIB_DIRECTORY);
   orderPluginsToMatchConfigFile(plugins, pluginHModules);
   processPlugins(buildSequence, plugins, pluginHModules, optionList);
   processCommandLineOptions(optionList, argc, argv);

   int error = executeBuildSequence(buildSequence, optionList, testFiles, sourceFiles, tempObjectFiles);

   // Since each of the CommandLineOptions in optionsList is part of a BuildSequenceStep
   // in buildSequence, we do not need a call to freeCommandLineOptions(optionsList).
   freeBuildSequence(buildSequence);
   freeFileLists(testFiles, sourceFiles, tempObjectFiles);
   freePluginList(plugins);
   return error;
}
