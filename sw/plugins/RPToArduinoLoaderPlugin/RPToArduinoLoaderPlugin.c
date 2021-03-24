#include "RPToArduinoLoaderPlugin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BuildSequenceStep* after_removeTempDir()
{
   BuildSequenceStep* step = malloc(sizeof(BuildSequenceStep));
   step->function_ptr = loadOntoArduinoWithAvrdude;
   strcpy(step->functionName, "loadOntoArduinoWithAvrdude");
   strcpy(step->option.optionText, "--load");
   strcpy(step->option.description, "Load the executable onto the Arduino");
   step->option.flagValue = false;
   return step;
}

int loadOntoArduinoWithAvrdude(
    const TestFileList* testFiles, const SourceFileList* sourceFiles, const ObjectFileList* tempObjectFiles, int errorOnPreviousStep, const char* basePath,
    const char* projectExecutableName)
{
   printf("Arduino Loader Plugin is Seeing the Executable Name: %s\n", projectExecutableName);
   return 0;
}