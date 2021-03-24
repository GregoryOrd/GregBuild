#ifndef BUILD_SEQUENCE_STEP
#define BUILD_SEQUENCE_STEP

#include "FileStructureDefs.h"
#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptionsStruct.h"

typedef  int (*STEP_FUNCTION)(const TestFileList* testFiles, const SourceFileList* sourceFiles,
                      const ObjectFileList* tempObjectFiles, int errorOnPreviousStep,
                      const char* basePath, const char* finalExecutableName);

typedef struct BuildSequenceStep {
  CommandLineOption option; // To run the function, the flagValue in here needs to be true
  STEP_FUNCTION function_ptr;
  char functionName[WINDOWS_MAX_PATH_LENGTH];
} BuildSequenceStep;

#ifdef __cplusplus
extern "C" {
#endif  

void copyBuildSequenceStep(BuildSequenceStep* dest, const BuildSequenceStep* src);
void setBuildSequenceStep(
    BuildSequenceStep* dest, const char* description, const char* optionText, bool flagValue, const STEP_FUNCTION function_ptr, const char* functionName);

#ifdef __cplusplus
}
#endif  

#endif