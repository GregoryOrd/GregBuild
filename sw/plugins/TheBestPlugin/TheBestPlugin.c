#include "TheBestPlugin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BuildSequenceStep* beforeLoadTestsAndSourceFiles()
{
   BuildSequenceStep* step = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   step->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
   step->option->optionText = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   step->option->description = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   step->option->flagValue = (bool*)malloc(sizeof(bool));
   step->function_ptr = printHelloWorld;
   step->functionName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   strcpy(step->functionName, "printHelloWorld");
   strcpy(step->option->optionText, "--best");
   strcpy(step->option->description, "TheBestPlugin");
   step->option->flagValue = (bool*)0;
   return step;
}

BuildSequenceStep* afterLoadTestsAndSourceFiles()
{
   BuildSequenceStep* step = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   step->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
   step->option->optionText = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   step->option->description = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   step->option->flagValue = (bool*)malloc(sizeof(bool));
   step->function_ptr = printHelloWorld;
   step->functionName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   strcpy(step->functionName, "printHelloWorld");
   strcpy(step->option->optionText, "--best");
   strcpy(step->option->description, "TheBestPlugin");
   step->option->flagValue = (bool*)0;
   return step;
}

int printHelloWorld(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath)
{
   printf("Hello World From The Best Plugin\n");
   return 0;
}