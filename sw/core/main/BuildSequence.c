#include "BuildSequence.h"

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions_ll.h"
#include "../application/CompileAndLinkCommands.h"
#include "../application/RunTests.h"
#include "../common/BuildSequenceStep.h"
#include "../common/GregBuildConstants.h"
#include "../fileSystemRecursion/FileAndTestCaseGatherer.h"
#include "../fileSystemRecursion/FileOperations.h"
#include "../testMainWriting/TestMainWriter.h"

void initBuildSequence(LinkedList* sequence)
{
   initEmptyLinkedList(sequence, BUILD_SEQUENCE_STEP_TYPE);
   setCoreBuildSequenceSteps(sequence);
}

void setCoreBuildSequenceSteps(LinkedList* sequence)
{
   BuildSequenceStep* loadTestsAndSourceFilesStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   allocateAndSetBuildSequenceStep(
       loadTestsAndSourceFilesStep, NULL_COMMAND_LINE_DESCRIPTION, NULL_COMMAND_LINE_OPTION_TEXT, NULL_COMMAND_LINE_FLAG_VALUE, loadTestsAndSourceFiles,
       "loadTestsAndSourceFiles");
   append_ll(sequence, loadTestsAndSourceFilesStep, BUILD_SEQUENCE_STEP_TYPE);

   BuildSequenceStep* compileIntoTempObjectFilesStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   allocateAndSetBuildSequenceStep(
       compileIntoTempObjectFilesStep, NULL_COMMAND_LINE_DESCRIPTION, NULL_COMMAND_LINE_OPTION_TEXT, NULL_COMMAND_LINE_FLAG_VALUE, compileIntoTempObjectFiles,
       "compileIntoTempObjectFiles");
   append_ll(sequence, compileIntoTempObjectFilesStep, BUILD_SEQUENCE_STEP_TYPE);

   BuildSequenceStep* linkObjectFilesWithGregTestDllToMakeProjectTestDllStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   allocateAndSetBuildSequenceStep(
       linkObjectFilesWithGregTestDllToMakeProjectTestDllStep, NO_TEST_DESCRIPTION, NO_TEST_OPTION_TEXT, NO_TEST_FLAG_VALUE,
       linkObjectFilesWithGregTestDllToMakeProjectTestDll, "linkObjectFilesWithGregTestDllToMakeProjectTestDll");
   append_ll(sequence, linkObjectFilesWithGregTestDllToMakeProjectTestDllStep, BUILD_SEQUENCE_STEP_TYPE);

   BuildSequenceStep* writeTestsToTestMainStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   allocateAndSetBuildSequenceStep(writeTestsToTestMainStep, NO_TEST_DESCRIPTION, NO_TEST_OPTION_TEXT, NO_TEST_FLAG_VALUE, writeTestsToTestMain, "writeTestsToTestMain");
   append_ll(sequence, writeTestsToTestMainStep, BUILD_SEQUENCE_STEP_TYPE);

   BuildSequenceStep* createTestMainExecutableFromProjectDllAndGregTestDllStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   allocateAndSetBuildSequenceStep(
       createTestMainExecutableFromProjectDllAndGregTestDllStep, NO_TEST_DESCRIPTION, NO_TEST_OPTION_TEXT, NO_TEST_FLAG_VALUE,
       createTestMainExecutableFromProjectDllAndGregTestDll, "createTestMainExecutableFromProjectDllAndGregTestDll");
   append_ll(sequence, createTestMainExecutableFromProjectDllAndGregTestDllStep, BUILD_SEQUENCE_STEP_TYPE);

   BuildSequenceStep* runTestsWithExitStatusCheckStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   allocateAndSetBuildSequenceStep(
       runTestsWithExitStatusCheckStep, NO_TEST_DESCRIPTION, NO_TEST_OPTION_TEXT, NO_TEST_FLAG_VALUE, runTestsWithExitStatusCheck, "runTestsWithExitStatusCheck");
   append_ll(sequence, runTestsWithExitStatusCheckStep, BUILD_SEQUENCE_STEP_TYPE);

   BuildSequenceStep* compileObjectFilesIntoProjectExecutableStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   allocateAndSetBuildSequenceStep(
       compileObjectFilesIntoProjectExecutableStep, NULL_COMMAND_LINE_DESCRIPTION, NULL_COMMAND_LINE_OPTION_TEXT, NULL_COMMAND_LINE_FLAG_VALUE,
       compileObjectFilesIntoProjectExecutable, "compileObjectFilesIntoProjectExecutable");
   append_ll(sequence, compileObjectFilesIntoProjectExecutableStep, BUILD_SEQUENCE_STEP_TYPE);

   BuildSequenceStep* removeTempDirStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   allocateAndSetBuildSequenceStep(removeTempDirStep, DELETE_TEMP_DIR_DESCRIPTION, DELETE_TEMP_DIR_OPTION_TEXT, DELETE_TEMP_DIR_FLAG_VALUE, removeTempDir, "removeTempDir");
   append_ll(sequence, removeTempDirStep, BUILD_SEQUENCE_STEP_TYPE);
}

int executeBuildSequence(LinkedList* buildSequence, LinkedList* options, TestFileList* testFiles, SourceFileList* sourceFiles, ObjectFileList* tempObjectFiles)
{
   int error = 0;
   char startingDirectory[WINDOWS_MAX_PATH_LENGTH] = SRC_DIR;

   for (int i = 0; i < buildSequence->size; i++)
   {
      BuildSequenceStep* step = (BuildSequenceStep*)at_ll(buildSequence, BUILD_SEQUENCE_STEP_TYPE, i);
      bool flagVal = flagValueForOption_ll(options, step->option->optionText, COMMAND_LINE_OPTION_TYPE);
      if (!error && flagVal)
      {
         printf("\n\n===============================================================\n");
         printf("Executing: %s\n", step->functionName);
         printf("===============================================================\n");
         error = (step->function_ptr)(testFiles, sourceFiles, tempObjectFiles, error, startingDirectory);
      }
   }

   return error;
}

void freeBuildSequence(LinkedList* sequence) { freeLinkedList(sequence, &freeBuildSequenceStep); }

void freeBuildSequenceStep(void* data)
{
   BuildSequenceStep* step = (BuildSequenceStep*)data;
   free(step->option->description);
   free(step->option->flagValue);
   free(step->option->optionText);
   free(step->option);
   free(step->functionName);
   free(step);
}