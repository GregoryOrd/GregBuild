#include "TimeTrackingPlugin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BuildSequenceStep* before_loadTestsAndSourceFiles()
{
   BuildSequenceStep* step = malloc(sizeof(BuildSequenceStep));
   step->option = malloc(sizeof(CommandLineOption));
   step->option->optionText = malloc(strlen("--time"));
   step->option->description = malloc(strlen("TimeTrackingPlugin"));
   step->function_ptr = printHelloWorld;
   step->functionName = malloc(strlen("printHelloWorld"));
   strcpy(step->functionName, "printHelloWorld");
   strcpy(step->option->optionText, "--time");
   strcpy(step->option->description, "TimeTrackingPlugin");
   step->option->flagValue = false;
   return step;
}

int printHelloWorld(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, char* basePath)
{
   printf("Hello World From The Time Tracking Plugin\n");
   return 0;
}