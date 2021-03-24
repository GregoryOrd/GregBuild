#include "RPToArduinoLoaderPlugin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/ExternalProgramExecution/ExternalProgramExecution.h"

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
   char flashInstruction[300] = "flash:w:dist/";
   strcat(flashInstruction, projectExecutableName);
   strcat(flashInstruction, ":e");

   char* const argv[] = {"sudo", "avrdude", "-v", "-p", "atmega328p", "-c", "atmelice_isp", "-P", "usb/001/002", "-b", "115200", "-U", flashInstruction};
   return popenChildProcess(13, argv);
}