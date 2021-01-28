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
#include "../testMainWriting/TestMainWriter.h"
#include "BuildSequence.h"

int main(int argc, char *argv[]) {
  printf("Starting GregBuild\n");
  LinkedList *optionList = (LinkedList *)malloc(sizeof(LinkedList));
  TestFileList *testFiles = NULL;
  SourceFileList *sourceFiles =
      (SourceFileList *)malloc(sizeof(SourceFileList));
  ObjectFileList *tempObjectFiles =
      (ObjectFileList *)malloc(sizeof(ObjectFileList));
  PluginList *plugins = (PluginList *)malloc(sizeof(PluginList));

  initAndProcessCommandLineOptions(optionList, argc, argv);
  if (flagValueForOption_ll(optionList, NO_TEST_OPTION_TEXT,
                            COMMAND_LINE_OPTION_TYPE)) {
    testFiles = (TestFileList *)malloc(sizeof(TestFileList));
  }
  initFileListsAndTempDir(testFiles, sourceFiles, tempObjectFiles);

  LinkedList *buildSequence = (LinkedList *)malloc(sizeof(LinkedList));
  initBuildSequence(buildSequence);

  initPluginList(plugins);
  loadPlugins(plugins, "./lib/plugins");
  processPlugins(buildSequence, plugins);

  int error = executeBuildSequence(buildSequence, optionList, testFiles,
                                   sourceFiles, tempObjectFiles);

  freeCommandLineOptions_ll(optionList);
  freeBuildSequence(buildSequence);
  freeFileLists(testFiles, sourceFiles, tempObjectFiles);
  freePluginList(plugins);
  return error;
}
