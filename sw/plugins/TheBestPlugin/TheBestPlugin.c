#include "TheBestPlugin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BuildSequenceStep* before_loadTestsAndSourceFiles()
{
   BuildSequenceStep* step = malloc(sizeof(BuildSequenceStep));
   step->option = malloc(sizeof(CommandLineOption));
   step->option->optionText = malloc(strlen("--best"));
   step->option->description = malloc("TheBestPlugin");
   step->function_ptr = printHelloWorld;
   step->functionName = malloc("printHelloWorld");
   strcpy(step->functionName, "printHelloWorld");
   strcpy(step->option->optionText, "--best");
   strcpy(step->option->description, "TheBestPlugin");
   step->option->flagValue = false;
   return step;
}

int printHelloWorld(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int errorOnPreviousStep, char* basePath)
{
   printf("Hello World From The Best Plugin\n");
   return 0;
}