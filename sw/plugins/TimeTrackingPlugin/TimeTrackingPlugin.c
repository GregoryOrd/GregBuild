#include "TimeTrackingPlugin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BuildSequenceStep* before_loadTestsAndSourceFiles()
{
   BuildSequenceStep* step = malloc(sizeof(BuildSequenceStep));
   step->function_ptr = printHelloWorld;
   strcpy(step->functionName, "printHelloWorld");
   strcpy(step->option.optionText, "--time");
   strcpy(step->option.description, "TimeTrackingPlugin");
   step->option.flagValue = false;
   return step;
}

int printHelloWorld(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath,
    const char* projectExecutableName)
{
   printf("Hello World From The Time Tracking Plugin\n");
   return 0;
}