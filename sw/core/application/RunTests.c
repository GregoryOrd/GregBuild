#include "RunTests.h"

#include <stdio.h>
#include <stdlib.h>

#include "../../external/GregCToolkit/sw/ExternalProgramExecution/CommandLineExecutables.h"
#include "../../external/GregCToolkit/sw/ExternalProgramExecution/ExternalProgramExecution.h"
#include "../../external/GregCToolkit/sw/FailureHandling/FailureHandling.h"
#include "../common/GregBuildConstants.h"
#include "CompileAndLinkCommands.h"

int runTests()
{
   copyDllsToCurrentDirectory();
   char* argv2[] = {TEMP_TEST_MAIN_EXE, NULL};
   int testResult = forkAndRunChildProcess(TEMP_TEST_MAIN_EXE, argv2);
   removeDllsFromCurrentDirectory();

   return testResult;
}

int runTestsWithExitStatusCheck(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);
   int retval = 1;
   int testResults = runTests();
   if (!testResults)
   {
      retval = 0;
   }
   else if (testResults == 139)
   {
      printf("\nSegmentation Fault While Running the Tests\n");
      printf("Build Failed");
   }
   return retval;
}

void copyDllsToCurrentDirectory()
{
   char* argv[] = {cp, TEMP_TEST_PROJECT_DLL, CURRENT_DIR, NULL};
   forkAndRunChildProcess(cp, argv);

   char* argv1[] = {cp, LIB_GREG_TEST_DLL, CURRENT_DIR, NULL};
   forkAndRunChildProcess(cp, argv1);
}

void removeDllsFromCurrentDirectory()
{
   char* argv[] = {rm, GREG_TEST_DLL, NULL};
   forkAndRunChildProcess(rm, argv);

   char* argv1[] = {rm, TEST_PROJECT_DLL, NULL};
   forkAndRunChildProcess(rm, argv1);
}