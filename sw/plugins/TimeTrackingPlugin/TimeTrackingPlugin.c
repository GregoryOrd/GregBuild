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

int printHelloWorld(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, char* basePath)
{
   printf("Hello World From The Time Tracking Plugin\n");
   return 0;
}