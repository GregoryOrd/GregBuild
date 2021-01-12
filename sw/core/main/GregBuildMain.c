#include "GregBuildMain.h"

#include "../application/CommandLineOptions.h"
#include "../application/CompileAndLinkCommands.h"
#include "../application/GregBuildConstants.h"
#include "../application/RunTests.h"
#include "../application/BuildSequence.h"
#include "../testMainWriting/TestMainWriter.h"
#include "../fileSystemRecursion/FileAndTestCaseGatherer.h"
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

    BuildSequence* buildSequence = (BuildSequence*)malloc(sizeof(BuildSequence));
    initBuildSequence(buildSequence);

    for(int i = 0; i < buildSequence->size; i++)
    {
        bool flagVal = flagValueForOption(options, buildSequence->steps[i].option->optionText);
        if(!error && flagVal)
        {
            error = (buildSequence->steps[i].function_ptr)(testFiles, sourceFiles, tempObjectFiles, error, startingDirectory);
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
    processCommandLineArgs(argc, argv, options);
    bool flag = *options->options[0].flagValue;
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
    removeDir(TEMP_DIR);
    if(testFiles != NULL)
    {
        freeTestFileList(testFiles);
    }
    freeSourceFileList(sourceFiles);
    freeObjectFileList(tempObjectFiles);
}