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
   loadTestsAndSourceFilesStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
   loadTestsAndSourceFilesStep->option->optionText = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   loadTestsAndSourceFilesStep->option->description = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   loadTestsAndSourceFilesStep->option->flagValue = (bool*)malloc(sizeof(bool));
   loadTestsAndSourceFilesStep->function_ptr = loadTestsAndSourceFiles;
   loadTestsAndSourceFilesStep->functionName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   strcpy(loadTestsAndSourceFilesStep->functionName, "loadTestsAndSourceFiles");
   strcpy(loadTestsAndSourceFilesStep->option->optionText, NULL_COMMAND_LINE_OPTION_TEXT);
   strcpy(loadTestsAndSourceFilesStep->option->description, NULL_COMMAND_LINE_DESCRIPTION);
   loadTestsAndSourceFilesStep->option->flagValue = (bool*)NULL_COMMAND_LINE_FLAG_VALUE;
   append_ll(sequence, loadTestsAndSourceFilesStep, BUILD_SEQUENCE_STEP_TYPE);

   BuildSequenceStep* compileIntoTempObjectFilesStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   compileIntoTempObjectFilesStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
   compileIntoTempObjectFilesStep->option->optionText = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   compileIntoTempObjectFilesStep->option->description = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   compileIntoTempObjectFilesStep->option->flagValue = (bool*)malloc(sizeof(bool));
   compileIntoTempObjectFilesStep->function_ptr = compileIntoTempObjectFiles;
   compileIntoTempObjectFilesStep->functionName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   strcpy(compileIntoTempObjectFilesStep->functionName, "compileIntoTempObjectFiles");
   strcpy(compileIntoTempObjectFilesStep->option->optionText, NULL_COMMAND_LINE_OPTION_TEXT);
   strcpy(compileIntoTempObjectFilesStep->option->description, NULL_COMMAND_LINE_DESCRIPTION);
   compileIntoTempObjectFilesStep->option->flagValue = (bool*)NULL_COMMAND_LINE_FLAG_VALUE;
   append_ll(sequence, compileIntoTempObjectFilesStep, BUILD_SEQUENCE_STEP_TYPE);

   BuildSequenceStep* linkObjectFilesWithGregTestDllToMakeProjectTestDllStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
   linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->option->optionText = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->option->description = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->option->flagValue = (bool*)malloc(sizeof(bool));
   linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->function_ptr = linkObjectFilesWithGregTestDllToMakeProjectTestDll;
   linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->functionName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   strcpy(linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->functionName, "linkObjectFilesWithGregTestDllToMakeProjectTestDll");
   strcpy(linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->option->optionText, NO_TEST_OPTION_TEXT);
   strcpy(linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->option->description, NO_TEST_DESCRIPTION);
   linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->option->flagValue = (bool*)NO_TEST_FLAG_VALUE;
   append_ll(sequence, linkObjectFilesWithGregTestDllToMakeProjectTestDllStep, BUILD_SEQUENCE_STEP_TYPE);

   BuildSequenceStep* writeTestsToTestMainStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   writeTestsToTestMainStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
   writeTestsToTestMainStep->option->optionText = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   writeTestsToTestMainStep->option->description = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   writeTestsToTestMainStep->option->flagValue = (bool*)malloc(sizeof(bool));
   writeTestsToTestMainStep->function_ptr = writeTestsToTestMain;
   writeTestsToTestMainStep->functionName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   strcpy(writeTestsToTestMainStep->functionName, "writeTestsToTestMain");
   strcpy(writeTestsToTestMainStep->option->optionText, NO_TEST_OPTION_TEXT);
   strcpy(writeTestsToTestMainStep->option->description, NO_TEST_DESCRIPTION);
   writeTestsToTestMainStep->option->flagValue = (bool*)NO_TEST_FLAG_VALUE;
   append_ll(sequence, writeTestsToTestMainStep, BUILD_SEQUENCE_STEP_TYPE);

   BuildSequenceStep* createTestMainExecutableFromProjectDllAndGregTestDllStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   createTestMainExecutableFromProjectDllAndGregTestDllStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
   createTestMainExecutableFromProjectDllAndGregTestDllStep->option->optionText = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   createTestMainExecutableFromProjectDllAndGregTestDllStep->option->description = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   createTestMainExecutableFromProjectDllAndGregTestDllStep->option->flagValue = (bool*)malloc(sizeof(bool));
   createTestMainExecutableFromProjectDllAndGregTestDllStep->function_ptr = createTestMainExecutableFromProjectDllAndGregTestDll;
   createTestMainExecutableFromProjectDllAndGregTestDllStep->functionName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   strcpy(createTestMainExecutableFromProjectDllAndGregTestDllStep->functionName, "createTestMainExecutableFromProjectDllAndGregTestDll");
   strcpy(createTestMainExecutableFromProjectDllAndGregTestDllStep->option->optionText, NO_TEST_OPTION_TEXT);
   strcpy(createTestMainExecutableFromProjectDllAndGregTestDllStep->option->description, NO_TEST_DESCRIPTION);
   createTestMainExecutableFromProjectDllAndGregTestDllStep->option->flagValue = (bool*)NO_TEST_FLAG_VALUE;
   append_ll(sequence, createTestMainExecutableFromProjectDllAndGregTestDllStep, BUILD_SEQUENCE_STEP_TYPE);

   BuildSequenceStep* runTestsWithExitStatusCheckStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   runTestsWithExitStatusCheckStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
   runTestsWithExitStatusCheckStep->option->optionText = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   runTestsWithExitStatusCheckStep->option->description = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   runTestsWithExitStatusCheckStep->option->flagValue = (bool*)malloc(sizeof(bool));
   runTestsWithExitStatusCheckStep->function_ptr = runTestsWithExitStatusCheck;
   runTestsWithExitStatusCheckStep->functionName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   strcpy(runTestsWithExitStatusCheckStep->functionName, "runTestsWithExitStatusCheck");
   strcpy(runTestsWithExitStatusCheckStep->option->optionText, NO_TEST_OPTION_TEXT);
   strcpy(runTestsWithExitStatusCheckStep->option->description, NO_TEST_DESCRIPTION);
   runTestsWithExitStatusCheckStep->option->flagValue = (bool*)NO_TEST_FLAG_VALUE;
   append_ll(sequence, runTestsWithExitStatusCheckStep, BUILD_SEQUENCE_STEP_TYPE);

   BuildSequenceStep* compileObjectFilesIntoProjectExecutableStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   compileObjectFilesIntoProjectExecutableStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
   compileObjectFilesIntoProjectExecutableStep->option->optionText = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   compileObjectFilesIntoProjectExecutableStep->option->description = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   compileObjectFilesIntoProjectExecutableStep->option->flagValue = (bool*)malloc(sizeof(bool));
   compileObjectFilesIntoProjectExecutableStep->function_ptr = compileObjectFilesIntoProjectExecutable;
   compileObjectFilesIntoProjectExecutableStep->functionName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   strcpy(compileObjectFilesIntoProjectExecutableStep->functionName, "compileObjectFilesIntoProjectExecutable");
   strcpy(compileObjectFilesIntoProjectExecutableStep->option->optionText, NULL_COMMAND_LINE_OPTION_TEXT);
   strcpy(compileObjectFilesIntoProjectExecutableStep->option->description, NULL_COMMAND_LINE_DESCRIPTION);
   compileObjectFilesIntoProjectExecutableStep->option->flagValue = (bool*)NULL_COMMAND_LINE_FLAG_VALUE;
   append_ll(sequence, compileObjectFilesIntoProjectExecutableStep, BUILD_SEQUENCE_STEP_TYPE);

   BuildSequenceStep* removeTempDirStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
   removeTempDirStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
   removeTempDirStep->option->optionText = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   removeTempDirStep->option->description = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   removeTempDirStep->option->flagValue = (bool*)malloc(sizeof(bool));
   removeTempDirStep->function_ptr = removeTempDir;
   removeTempDirStep->functionName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   strcpy(removeTempDirStep->functionName, "removeTempDir");
   strcpy(removeTempDirStep->option->optionText, DELETE_TEMP_DIR_OPTION_TEXT);
   strcpy(removeTempDirStep->option->description, DELETE_TEMP_DIR_DESCRIPTION);
   removeTempDirStep->option->flagValue = (bool*)DELETE_TEMP_DIR_FLAG_VALUE;
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
