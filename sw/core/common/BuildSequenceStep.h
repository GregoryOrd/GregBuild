#ifndef BUILD_SEQUENCE_STEP
#define BUILD_SEQUENCE_STEP

#include "FileStructureDefs.h"
#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptionsStruct.h"

typedef struct BuildSequenceStep {
  CommandLineOption *option; // To run the function, the flagValue in here needs to be true
  int (*function_ptr)(TestFileList *testFiles, SourceFileList *sourceFiles,
                      ObjectFileList *tempObjectFiles, int previousStepFailed,
                      char *basePath);
  char* functionName;
} BuildSequenceStep;

#endif