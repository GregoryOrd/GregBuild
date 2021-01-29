#include "TimeTrackingPlugin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BuildSequenceStep *beforeLoadingTestAndSourceFiles() {
  BuildSequenceStep *step =
      (BuildSequenceStep *)malloc(sizeof(BuildSequenceStep));
  step->option = (CommandLineOption *)malloc(sizeof(CommandLineOption));
  step->option->optionText = (char *)malloc(sizeof(char));
  step->option->description = (char *)malloc(sizeof(char));
  step->option->flagValue = (bool *)malloc(sizeof(bool));
  step->function_ptr = printHelloWorld;
  strcpy(step->option->optionText, "--time");
  strcpy(step->option->description, "TimeTrackingPlugin");
  step->option->flagValue = (bool *)0;
  return step;
}

BuildSequenceStep *afterLoadingTestAndSourceFiles() {
  BuildSequenceStep *step =
      (BuildSequenceStep *)malloc(sizeof(BuildSequenceStep));
  step->option = (CommandLineOption *)malloc(sizeof(CommandLineOption));
  step->option->optionText = (char *)malloc(sizeof(char));
  step->option->description = (char *)malloc(sizeof(char));
  step->option->flagValue = (bool *)malloc(sizeof(bool));
  step->function_ptr = printHelloWorld;
  strcpy(step->option->optionText, "time");
  strcpy(step->option->description, "TimeTrackingPlugin");
  step->option->flagValue = (bool *)0;
  return step;
}

int printHelloWorld(TestFileList *testFiles, SourceFileList *sourceFiles,
                    ObjectFileList *tempObjectFiles, int previousStepFailed,
                    char *basePath) {
  printf("Hello World From The Time Tracking Plugin\n");
  return 0;
}