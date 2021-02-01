#ifndef BUILD_SEQUENCE_STEP
#define BUILD_SEQUENCE_STEP

#include "FileStructureDefs.h"
#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptionsStruct.h"

typedef  int (*STEP_FUNCTION)(const TestFileList *testFiles, const SourceFileList *sourceFiles,
                      const ObjectFileList *tempObjectFiles, int previousStepFailed,
                      char *basePath);

typedef struct BuildSequenceStep {
  CommandLineOption *option; // To run the function, the flagValue in here needs to be true
  STEP_FUNCTION function_ptr;
  char* functionName;
} BuildSequenceStep;

void allocateAndCopyBuildSequenceStep(BuildSequenceStep* dest, BuildSequenceStep* src);
void allocateAndSetBuildSequenceStep(
    BuildSequenceStep* dest, const char* description, const char* optionText, const bool* flagValue, const STEP_FUNCTION function_ptr, const char* functionName);

#endif