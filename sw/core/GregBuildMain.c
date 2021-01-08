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
    initCommandLineOptions(options);
    processCommandLineArgs(argc, argv, options);
    if(!options->runTests)
    {
        printf("No Test Build");
    }

    int error = 0;
    char startingDirectory[WINDOWS_MAX_PATH_LENGTH] = SRC_DIR;

    TestFileList* testFiles =  NULL;
    if(options->runTests)
    {
        testFiles = (TestFileList*)malloc(sizeof(TestFileList));
        initTestFiles(testFiles);
    }

    SourceFileList* sourceFiles = (SourceFileList*)malloc(sizeof(SourceFileList));
    ObjectFileList* tempObjectFiles = (ObjectFileList*)malloc(sizeof(ObjectFileList));
    initFileListsAndTempDir(testFiles, sourceFiles, tempObjectFiles);

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

    free(options);
    removeTempDirAndFreeFileLists(testFiles, sourceFiles, tempObjectFiles);
    return error;
}

void exitIfPreviousStepFailed(int previousStepFailed)
{
    if(previousStepFailed)
    {
        exit(1);
    }
}

void initFileListsAndTempDir(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles)
{
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