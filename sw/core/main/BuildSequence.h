#ifndef BUILD_SEQUENCE_H
#define BUILD_SEQUENCE_H

#include "../common/BuildSequenceStep.h"
#include "../common/FileStructureDefs.h"
#include "../../external/GregCToolkit/sw/Collections/LinkedList.h"

void initBuildSequence(LinkedList *sequence);
void setCoreBuildSequenceSteps(LinkedList *sequence);
int executeBuildSequence(LinkedList *buildSequence, LinkedList *options,
                         TestFileList *testFiles, SourceFileList *sourceFiles,
                         ObjectFileList *tempObjectFiles);
void freeBuildSequence(LinkedList *sequence);
void freeBuildSequenceStep(void *data);

#endif