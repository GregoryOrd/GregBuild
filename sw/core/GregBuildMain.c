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

    if(!error)
    {
        error = (*loadTestsAndSourceFiles_func_ptr)(testFiles, sourceFiles, tempObjectFiles, error, startingDirectory);
        printf("Source Files Size: %d\n", sourceFiles->size);
    }
    if(!error)
    {
        error = (*compileIntoTempObjectFiles_func_ptr)(testFiles, sourceFiles, tempObjectFiles, error, NULL);
    }
    // if(!error)
    // {
    //     error = (*linkObjectFilesWithGregTestDllToMakeProjectTestDll_func_ptr)(testFiles, sourceFiles, tempObjectFiles, error, NULL);
    //     exitIfPreviousStepFailed(error);
    // }
    // if(!error && options->runTests)
    // {
    //    error = (*writeTestsToTestMain_func_ptr)(testFiles, sourceFiles, tempObjectFiles, error, NULL);
    //    exitIfPreviousStepFailed(error);
    // }
    // if(!error && options->runTests)
    // {
    //    error = (*createTestMainExecutableFromProjectDllAndGregTestDll_func_ptr)(testFiles, sourceFiles, tempObjectFiles, error, NULL);
    //    exitIfPreviousStepFailed(error);
    // }
    // if(!error && options->runTests)
    // {
    //     error = (*runTestsWithExitStatusCheck_func_ptr)(testFiles, sourceFiles, tempObjectFiles, error, NULL);
    //     exitIfPreviousStepFailed(error);
    // }
    // if(!error)
    // {
    //     error = (*compileObjectFilesIntoProjectExecutable_func_ptr)(testFiles, sourceFiles, tempObjectFiles, error, NULL);
    //     exitIfPreviousStepFailed(error);
    // }

    free(options);
    removeTempDirAndFreeFileLists(testFiles, sourceFiles, tempObjectFiles);
    return error;
}

void exitIfPreviousStepFailed(int previousStepFailed)
{
    if(previousStepFailed)
    {
        printf("Error. Exiting");
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