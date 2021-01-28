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
  strcpy(step->option->optionText, "TimeTrackingPlugin_TEXT");
  strcpy(step->option->description, "TimeTrackingPlugin_DESCRIPTION");
  step->option->flagValue = (bool *)NULL_COMMAND_LINE_FLAG_VALUE;
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
  strcpy(step->option->optionText, NULL_COMMAND_LINE_OPTION_TEXT);
  strcpy(step->option->description, NULL_COMMAND_LINE_DESCRIPTION);
  step->option->flagValue = (bool *)NULL_COMMAND_LINE_FLAG_VALUE;
  return step;
}

int printHelloWorld(TestFileList *testFiles, SourceFileList *sourceFiles,
                    ObjectFileList *tempObjectFiles, int previousStepFailed,
                    char *basePath) {
  printf("Hello World From The Time Tracking Plugin\n");
  return 0;
}