#include "GregBuildMain.h"

#include "CompileAndLinkCommands.h"
#include "FileSystemOperations.h"
#include "GregBuildConstants.h"
#include "RunTests.h"
#include "../testMainWriting/TestMainWriter.h"
#include "../fileSystemRecursion/FileAndTestCaseGatherer.h"

int main()
{
    int retval = 1;
    char startingDirectory[WINDOWS_MAX_PATH_LENGTH] = SRC_DIR;

    TestFileList* testFiles = (TestFileList*)malloc(sizeof(TestFileList));
    SourceFileList* sourceFiles = (SourceFileList*)malloc(sizeof(SourceFileList));
    ObjectFileList* tempObjectFiles = (ObjectFileList*)malloc(sizeof(ObjectFileList));
    initFileListsAndTempDir(testFiles, sourceFiles, tempObjectFiles);

    loadTestsAndSourceFiles(testFiles, sourceFiles, startingDirectory);
    writeTestsToTestMain(testFiles);

    compileIntoTempObjectFiles(tempObjectFiles, testFiles, sourceFiles);
    linkObjectFilesWithGregTestDllToMakeProjectTestDll(tempObjectFiles);
    createTestMainExecutableFromProjectDllAndGregTestDll();

    retval = runTestsAndCompileIfTheyPass(tempObjectFiles);
    compileObjectFilesIntoProjectExecutable(tempObjectFiles, retval);
    removeTempDirAndFreeFileLists(testFiles, sourceFiles, tempObjectFiles);
    return retval;
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
    freeTestFileList(testFiles);
    freeSourceFileList(sourceFiles);
    freeObjectFileList(tempObjectFiles);
}