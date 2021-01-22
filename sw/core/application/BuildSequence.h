#ifndef BUILD_SEQUENCE_H
#define BUILD_SEQUENCE_H

#include "../fileSystemRecursion/FileStructureDefs.h"
#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions.h"
#include "../../external/GregCToolkit/sw/Collections/LinkedList.h"

typedef struct BuildSequenceStep
{
    CommandLineOption* option; //To run the function, the flagValue in here needs to be true
    int (*function_ptr)(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath);
} BuildSequenceStep;

void initBuildSequence(LinkedList* sequence);
void setCoreBuildSequenceSteps(LinkedList* sequence);
int executeBuildSequence(LinkedList* buildSequence, CommandLineOptionList* options, TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles);
void freeBuildSequence(LinkedList* sequence);
void freeBuildSequenceStep(void* data);

#endif