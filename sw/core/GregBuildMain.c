#include "GregBuildMain.h"

#include "CommandLineOptions.h"
#include "CompileAndLinkCommands.h"
#include "FileSystemOperations.h"
#include "GregBuildConstants.h"
#include "RunTests.h"
#include "../testMainWriting/TestMainWriter.h"
#include "../fileSystemRecursion/FileAndTestCaseGatherer.h"

int main(int argc, char *argv[])
{
    CommandLineOptions* options = (CommandLineOptions*)malloc(sizeof(CommandLineOptions));
    initCommandLineOptions(options);
    processCommandLineArgs(argc, argv, options);

    int retval = 0;
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

    loadTestsAndSourceFiles(testFiles, sourceFiles, startingDirectory);

    compileIntoTempObjectFiles(tempObjectFiles, testFiles, sourceFiles);
    linkObjectFilesWithGregTestDllToMakeProjectTestDll(tempObjectFiles);

    if(options->runTests)
    {
        writeTestsToTestMain(testFiles);
        createTestMainExecutableFromProjectDllAndGregTestDll();
        retval = runTestsAndCompileIfTheyPass(tempObjectFiles);
    }
    else
    {
        printf("No Test Build");
    }
    

    retval = compileObjectFilesIntoProjectExecutable(tempObjectFiles, retval);

    free(options);
    removeTempDirAndFreeFileLists(testFiles, sourceFiles, tempObjectFiles);
    return retval;
}

void initCommandLineOptions(CommandLineOptions* options)
{
    options->runTests = true;
}

void processCommandLineArgs(int arc, char* argv[], CommandLineOptions* options)
{
    for(int i = 1; i < arc; i++)
    {
        bool optionFound = false;
        if(strcmp(argv[i], NO_TEST_OPTION_TEXT) == 0)
        {
            options->runTests = false;
            optionFound = true;
        }
        if(!optionFound)
        {
            printf("Unrecognized command line option provided: %s\n\n", argv[1]);
            printf("Supported Options:\n");
            for(int optionNum = 0; optionNum < sizeof(optionsList) / sizeof(char*); optionNum++)
            {
                printf("%s    %s\n", optionsList[optionNum], descriptionsList[optionNum]);
            }
            exit(1);
        }
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