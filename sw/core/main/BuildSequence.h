#ifndef BUILD_SEQUENCE_H
#define BUILD_SEQUENCE_H

#include "../common/BuildSequenceStep.h"
#include "../common/FileStructureDefs.h"
#include "../../external/GregCToolkit/sw/Collections/LinkedList/LinkedList.h"
#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions.h"

#ifdef __cplusplus
extern "C" {
#endif  

void initBuildSequence(LinkedList *sequence);
void setCoreBuildSequenceSteps(LinkedList *sequence);
int executeBuildSequence(const LinkedList *buildSequence, const CommandLineOptionList *options,
                         TestFileList *testFiles, SourceFileList *sourceFiles,
                         ObjectFileList *tempObjectFiles);
void printBuildSequenceExecutionMessage(BuildSequenceStep* step);
void freeBuildSequence(LinkedList *sequence);
void freeBuildSequenceStep(void *data);

#ifdef __cplusplus
}
#endif  

#endif