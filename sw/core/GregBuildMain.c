#include "GregBuildMain.h"

#include "CommandLineOptions.h"
#include "CompileAndLinkCommands.h"
#include "FileSystemOperations.h"
#include "GregBuildConstants.h"
#include "RunTests.h"
#include "Sequence.h"
#include "../testMainWriting/TestMainWriter.h"
#include "../fileSystemRecursion/FileAndTestCaseGatherer.h"

int main(int argc, char *argv[])
{
    CommandLineOptions* options = (CommandLineOptions*)malloc(sizeof(CommandLineOptions));
    TestFileList* testFiles =  NULL;
    SourceFileList* sourceFiles = (SourceFileList*)malloc(sizeof(SourceFileList));
    ObjectFileList* tempObjectFiles = (ObjectFileList*)malloc(sizeof(ObjectFileList));

    initAndProcessCommandLineOptions(options, argc, argv);
    if(options->runTests)
    {
        testFiles = (TestFileList*)malloc(sizeof(TestFileList));
    }
    initFileListsAndTempDir(testFiles, sourceFiles, tempObjectFiles);

    int error = executeSequence(options, testFiles, sourceFiles, tempObjectFiles);

    free(options);
    removeTempDirAndFreeFileLists(testFiles, sourceFiles, tempObjectFiles);
    return error;
}

int executeSequence(CommandLineOptions* options, TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles)
{
    int error = 0;
    char startingDirectory[WINDOWS_MAX_PATH_LENGTH] = SRC_DIR;

    for(int i = 0; i < NUM_FUNCTIONS_IN_SEQUENCE; i++)
    {
        if(!error)
        {
            if(i >= 3 && i <= 5)
            {
                if(options->runTests)
                {
                    error = (sequence[i])(testFiles, sourceFiles, tempObjectFiles, error, startingDirectory);
                }
            }
            else
            {
                error = (sequence[i])(testFiles, sourceFiles, tempObjectFiles, error, startingDirectory);
            }
        }
    }
    return error;
}

void exitIfPreviousStepFailed(int previousStepFailed)
{
    if(previousStepFailed)
    {
        exit(1);
    }
}

void initAndProcessCommandLineOptions(CommandLineOptions* options, int argc, char* argv[])
{
    initCommandLineOptions(options);
    processCommandLineArgs(argc, argv, options);
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