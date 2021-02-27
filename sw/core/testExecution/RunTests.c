#include "RunTests.h"

#include <stdio.h>
#include <stdlib.h>

#include "../../external/GregCToolkit/sw/ExternalProgramExecution/CommandLineExecutables.h"
#include "../../external/GregCToolkit/sw/ExternalProgramExecution/ExternalProgramExecution.h"
#include "../../external/GregCToolkit/sw/FailureHandling/FailureHandling.h"
#include "../application/CompileAndLinkCommands.h"
#include "../common/GregBuildConstants.h"
#include "../common/global/GlobalVariables.h"

int runTests()
{
   copyDllsToCurrentDirectory();
   char* argv[] = {TEMP_TEST_MAIN_EXE, NULL};
   int testResult = forkAndRunChildProcess(argv);
   removeDllsFromCurrentDirectory();
   setTestsWereRun();
   return testResult;
}

int runTestsWithExitStatusCheck(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath)
{
   exitIfError(errorOnPreviousStep);
   if (tempObjectFiles->size > 0)
   {
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

   return 0;
}

void copyDllsToCurrentDirectory()
{
   char* argv[] = {cp, TEMP_TEST_PROJECT_LIBRARY, CURRENT_DIR, NULL};
   forkAndRunChildProcess(argv);

   char* argv1[] = {cp, LIB_GREG_TEST_LIBRARY, CURRENT_DIR, NULL};
   forkAndRunChildProcess(argv1);
}

void removeDllsFromCurrentDirectory()
{
   char* argv[] = {rm, GREG_TEST_LIBRARY, NULL};
   forkAndRunChildProcess(argv);

   char* argv1[] = {rm, TEST_PROJECT_LIBRARY, NULL};
   forkAndRunChildProcess(argv1);
}