#include "BuildSequence.h"
#include "../application/CompileAndLinkCommands.h"
#include "../application/CoreCommandLineOptions.h"
#include "../application/RunTests.h"
#include "../common/FileStructureDefs.h"
#include "../common/GregBuildConstants.h"
#include "../testMainWriting/TestMainWriter.h"
#include "../fileSystemRecursion/FileAndTestCaseGatherer.h"
#include "../fileSystemRecursion/FileOperations.h"
#include "../../external/GregCToolkit/sw/Collections/LinkedList.h"
#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"

int main(int argc, char *argv[])
{
    printf("Starting GregBuild\n");
    CommandLineOptionList* optionList = (CommandLineOptionList*)malloc(sizeof(CommandLineOptionList));
    TestFileList* testFiles =  NULL;
    SourceFileList* sourceFiles = (SourceFileList*)malloc(sizeof(SourceFileList));
    ObjectFileList* tempObjectFiles = (ObjectFileList*)malloc(sizeof(ObjectFileList));

    initAndProcessCommandLineOptions(optionList, argc, argv);
    if(flagValueForOption(optionList, NO_TEST_OPTION_TEXT))
    {
        testFiles = (TestFileList*)malloc(sizeof(TestFileList));
    }
    initFileListsAndTempDir(testFiles, sourceFiles, tempObjectFiles);

    LinkedList* buildSequence = (LinkedList*)malloc(sizeof(LinkedList));
    initBuildSequence(buildSequence);
    int error = executeBuildSequence(buildSequence, optionList, testFiles, sourceFiles, tempObjectFiles);

    freeCommandLineOptions(optionList);
    freeBuildSequence(buildSequence);
    freeFileLists(testFiles, sourceFiles, tempObjectFiles);
    return error;
}