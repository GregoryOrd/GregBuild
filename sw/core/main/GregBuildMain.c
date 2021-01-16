#include "GregBuildMain.h"

#include "../application/CompileAndLinkCommands.h"
#include "../application/GregBuildConstants.h"
#include "../application/RunTests.h"
#include "../application/BuildSequence.h"
#include "../testMainWriting/TestMainWriter.h"
#include "../fileSystemRecursion/FileAndTestCaseGatherer.h"
#include "../../external/GregCToolkit/sw/Collections/LinkedList.h"
#include "../../external/GregCToolkit/sw/FileSystem/ManageDirectories.h"

int main(int argc, char *argv[])
{
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

    int error = executeBuildSequence(optionList, testFiles, sourceFiles, tempObjectFiles);

    freeCommandLineOptions(optionList);
    removeTempDirAndFreeFileLists(testFiles, sourceFiles, tempObjectFiles);
    return error;
}

int executeBuildSequence(CommandLineOptionList* options, TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles)
{
    int error = 0;
    char startingDirectory[WINDOWS_MAX_PATH_LENGTH] = SRC_DIR;

    LinkedList* buildSequence = (LinkedList*)malloc(sizeof(LinkedList));
    initBuildSequence(buildSequence);

    for(int i = 0; i < buildSequence->size; i++)
    {
        BuildSequenceStep* step = (BuildSequenceStep*)at_ll(buildSequence, BUILD_SEQUENCE_STEP_TYPE, i);
        bool flagVal = flagValueForOption(options,  step->option->optionText);
        if(!error && flagVal)
        {
            error = (step->function_ptr)(testFiles, sourceFiles, tempObjectFiles, error, startingDirectory);
        }
    }
    free(buildSequence);
    return error;
}

void exitIfPreviousStepFailed(int previousStepFailed)
{
    if(previousStepFailed)
    {
        exit(1);
    }
}

void initAndProcessCommandLineOptions(CommandLineOptionList* options, int argc, char* argv[])
{
    initCommandLineOptions(options);
    setCoreCommandLineOptions(options);
    processCommandLineArgs(argc, argv, options);
    coreCommandLineAcknowldegmentPrintouts(options);
}

void setCoreCommandLineOptions(CommandLineOptionList* list)
{
    strcpy(list->options[0].description, NO_TEST_DESCRIPTION);
    strcpy(list->options[0].optionText, NO_TEST_OPTION_TEXT);
    *list->options[0].flagValue = NO_TEST_FLAG_VALUE;
}

void coreCommandLineAcknowldegmentPrintouts(CommandLineOptionList* list)
{
    bool flag = flagValueForOption(list, NO_TEST_OPTION_TEXT);
    if(!flag)
    {
        printf("No Test Build\n");
    }
}

void initFileListsAndTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles)
{
    initTestFiles(testFiles);
    initSourceFiles(sourceFiles);
    initObjectFileList(tempObjectFiles);
    makeDir(TEMP_DIR);
}

void initObjectFileList(ObjectFileList* objectFiles)
{
    objectFiles->size = 0;
    objectFiles->files = (ObjectFile*)malloc(sizeof(ObjectFile));
    objectFiles->files[0].name = NULL;
}

void freeObjectFileList(ObjectFileList* list)
{
    for(int i = 0; i < list->size; i++)
    {
        free(list->files[i].name);
    }
    free(list);
}

void removeTempDirAndFreeFileLists(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles)
{
    //removeDir(TEMP_DIR);
    if(testFiles != NULL)
    {
        freeTestFileList(testFiles);
    }
    freeSourceFileList(sourceFiles);
    freeObjectFileList(tempObjectFiles);
}