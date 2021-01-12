#ifndef BUILD_SEQUENCE_H
#define BUILD_SEQUENCE_H

#include "CompileAndLinkCommands.h"
#include "RunTests.h"
#include "../fileSystemRecursion/FileAndTestCaseGatherer.h"
#include "../testMainWriting/TestMainWriter.h"
#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions.h"

typedef struct BuildSequenceStep
{
    CommandLineOption* option; //To run the function, the flagValue in here needs to be true
    int (*function_ptr)(TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles, int previousStepFailed, char* basePath);
} BuildSequenceStep;

typedef struct BuildSequence
{
    int size;
    BuildSequenceStep* steps;
} BuildSequence;

void initBuildSequence(BuildSequence* sequence);
void freeBuildSequence(BuildSequence* sequence);

#endif