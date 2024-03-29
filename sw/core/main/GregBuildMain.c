#include "../../external/GregCToolkit/sw/Collections/LinkedList/LinkedList.h"
#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"
#include "../common/FileOperations.h"
#include "../common/FileStructureDefs.h"
#include "../common/GregBuildConstants.h"
#include "../common/global/GlobalVariables.h"
#include "../compiler/CompileAndLinkCommands.h"
#include "../fileSystemRecursion/FileAndTestCaseGatherer.h"
#include "../pluginLoader/PluginLoader.h"
#include "../pluginLoader/PluginProcessor.h"
#include "../testExecution/RunTests.h"
#include "../testExecution/TestMainWriter.h"
#include "BuildSequence.h"
#include "CoreCommandLineOptions.h"

int main(int argc, const char* argv[])
{
   printf("Starting GregBuild\n");
   CommandLineOptionList* optionList = malloc(sizeof(CommandLineOptionList));
   TestFileList* testFiles = malloc(sizeof(TestFileList));
   SourceFileList* sourceFiles = malloc(sizeof(SourceFileList));
   ObjectFileList* tempObjectFiles = malloc(sizeof(ObjectFileList));
   PluginList* plugins = malloc(sizeof(PluginList));
   LinkedList* pluginModules = malloc(sizeof(LinkedList));
   LinkedList* buildSequence = malloc(sizeof(LinkedList));

   initCoreCommandLineOptions(optionList);
   initFileListsAndTempDir(testFiles, sourceFiles, tempObjectFiles);
   initBuildSequence(buildSequence);

   loadPlugins(plugins, pluginModules, PLUGINS_LIB_DIRECTORY);
   processPlugins(buildSequence, plugins, pluginModules, optionList);
   processCommandLineOptions(optionList, argc, argv);

   int error = executeBuildSequence(buildSequence, optionList, testFiles, sourceFiles, tempObjectFiles);

   freeCommandLineOptions(optionList);
   freeFileLists(testFiles, sourceFiles, tempObjectFiles);
   freePluginList(plugins);
   freePluginModules(pluginModules);
   freeBuildSequence(buildSequence);
   freeGlobalOptionsLists();
   freeHostExcludedFilesList();
   freeTargetExcludedFilesList();
   freeHardwareSimulationLibrariesList();
   return error;
}
