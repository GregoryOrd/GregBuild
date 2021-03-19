#include "BuildSequence.h"

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions.h"
#include "../../external/GregCToolkit/sw/String/StringUtils.h"
#include "../common/BuildSequenceStep.h"
#include "../common/GregBuildConstants.h"
#include "../common/global/GlobalVariables.h"
#include "CoreBuildSequence.h"

void initBuildSequence(LinkedList* sequence)
{
   initEmptyLinkedList(sequence, BUILD_SEQUENCE_STEP_TYPE);
   setCoreBuildSequenceSteps(sequence);
}

void setCoreBuildSequenceSteps(LinkedList* sequence)
{
   for (int i = 0; i < (sizeof(coreBuildSequenceInfo) / sizeof(BuildSequenceStepInfo)); i++)
   {
      BuildSequenceStep* step = malloc(sizeof(BuildSequenceStep));
      BuildSequenceStepInfo info = coreBuildSequenceInfo[i];
      setBuildSequenceStep(step, info.description, info.optionText, info.flagValue, info.function_ptr, info.functionName);
      append_ll(sequence, step, BUILD_SEQUENCE_STEP_TYPE);
   }
}

int executeBuildSequence(
    const LinkedList* buildSequence, const CommandLineOptionList* options, TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles)
{
   int error = 0;
   for (int i = 0; i < buildSequence->size; i++)
   {
      BuildSequenceStep* step = (BuildSequenceStep*)at_ll(buildSequence, BUILD_SEQUENCE_STEP_TYPE, i);

      bool flagVal = flagValueForOption(options, step->option.optionText);
      if (!error && flagVal)
      {
         printBuildSequenceExecutionMessage(step);
         error = (step->function_ptr)(testFiles, sourceFiles, tempObjectFiles, error, buildStartingDirectory());
      }
   }

   return error;
}

void printBuildSequenceExecutionMessage(BuildSequenceStep* step)
{
   bool skippedTestStep = (stringsAreEqual(step->option.optionText, NO_TEST_OPTION_TEXT) && hostCompileFailed());
   if (!skippedTestStep)
   {
      printf("\n===================================================================\n");
      printf("%s\n", step->functionName);
      printf("====================================================================\n");
   }
}

void freeBuildSequence(LinkedList* sequence) { freeLinkedList(sequence, &freeBuildSequenceStep); }

void freeBuildSequenceStep(void* buildSequenceStep) { free(buildSequenceStep); }