#include "RunTests.h"

#include "CompileAndLinkCommands.h"
#include "FileSystemOperations.h"
#include "GregBuildConstants.h"
#include "../commandLineCalls/ExternalProgramExecution.h"
#include "../fileSystemRecursion/ObjectFileStructureDefs.h"
#include "../main/GregBuildMain.h"

#include <stdio.h>
#include <stdlib.h>

int runTests()
{
    copyDllsToCurrentDirectory();
    char * const argv2[] = {TEMP_TEST_MAIN_EXE, NULL};
    int testResult = forkAndRunChildProcess(TEMP_TEST_MAIN_EXE, argv2); 
    removeDllsFromCurrentDirectory(); 

    return testResult;
}

int runTestsWithExitStatusCheck(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath)
{
    exitIfPreviousStepFailed(previousStepFailed);
    int retval = 1;
    int testResults = runTests();
    if(!testResults)
    {   
        retval = 0;
    }
    else if(testResults == 139)
    {
        printf("\nSegmentation Fault While Running the Tests\n");
        printf("Build Failed");
    }
    return retval;
}