#include "BuildSequence.h"

#include "../application/GregBuildConstants.h"

const int NUM_CORE_SEQUENCE_STEPS = 7;

void initBuildSequence(LinkedList* sequence)
{
    initEmptyLinkedList(sequence, BUILD_SEQUENCE_STEP_TYPE);

    BuildSequenceStep* loadTestsAndSourceFilesStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
    loadTestsAndSourceFilesStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
    loadTestsAndSourceFilesStep->option->optionText = (char*)malloc(sizeof(char));
    loadTestsAndSourceFilesStep->option->description = (char*)malloc(sizeof(char));
    loadTestsAndSourceFilesStep->option->flagValue = (bool*)malloc(sizeof(bool)); 
    loadTestsAndSourceFilesStep->function_ptr = loadTestsAndSourceFiles;
    strcpy(loadTestsAndSourceFilesStep->option->optionText, NULL_COMMAND_LINE_OPTION_TEXT);
    strcpy(loadTestsAndSourceFilesStep->option->description, NULL_COMMAND_LINE_DESCRIPTION);
    loadTestsAndSourceFilesStep->option->flagValue = (bool*)NULL_COMMAND_LINE_FLAG_VALUE;
    append_ll(sequence, loadTestsAndSourceFilesStep, BUILD_SEQUENCE_STEP_TYPE);


    BuildSequenceStep* compileIntoTempObjectFilesStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
    compileIntoTempObjectFilesStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
    compileIntoTempObjectFilesStep->option->optionText = (char*)malloc(sizeof(char));
    compileIntoTempObjectFilesStep->option->description = (char*)malloc(sizeof(char));
    compileIntoTempObjectFilesStep->option->flagValue = (bool*)malloc(sizeof(bool)); 
    compileIntoTempObjectFilesStep->function_ptr = compileIntoTempObjectFiles;
    strcpy(compileIntoTempObjectFilesStep->option->optionText, NULL_COMMAND_LINE_OPTION_TEXT);
    strcpy(compileIntoTempObjectFilesStep->option->description, NULL_COMMAND_LINE_DESCRIPTION);
    compileIntoTempObjectFilesStep->option->flagValue = (bool*)NULL_COMMAND_LINE_FLAG_VALUE;
    append_ll(sequence, compileIntoTempObjectFilesStep, BUILD_SEQUENCE_STEP_TYPE);


    BuildSequenceStep* linkObjectFilesWithGregTestDllToMakeProjectTestDllStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
    linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
    linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->option->optionText = (char*)malloc(sizeof(char));
    linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->option->description = (char*)malloc(sizeof(char));
    linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->option->flagValue = (bool*)malloc(sizeof(bool)); 
    linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->function_ptr = linkObjectFilesWithGregTestDllToMakeProjectTestDll;
    strcpy(linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->option->optionText, NO_TEST_OPTION_TEXT);
    strcpy(linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->option->description, NO_TEST_DESCRIPTION);
    linkObjectFilesWithGregTestDllToMakeProjectTestDllStep->option->flagValue = (bool*)NO_TEST_FLAG_VALUE;
    append_ll(sequence, linkObjectFilesWithGregTestDllToMakeProjectTestDllStep, BUILD_SEQUENCE_STEP_TYPE);


    BuildSequenceStep* writeTestsToTestMainStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
    writeTestsToTestMainStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
    writeTestsToTestMainStep->option->optionText = (char*)malloc(sizeof(char));
    writeTestsToTestMainStep->option->description = (char*)malloc(sizeof(char));
    writeTestsToTestMainStep->option->flagValue = (bool*)malloc(sizeof(bool)); 
    writeTestsToTestMainStep->function_ptr = writeTestsToTestMain;
    strcpy(writeTestsToTestMainStep->option->optionText, NO_TEST_OPTION_TEXT);
    strcpy(writeTestsToTestMainStep->option->description, NO_TEST_DESCRIPTION);
    writeTestsToTestMainStep->option->flagValue = (bool*)NO_TEST_FLAG_VALUE;
    append_ll(sequence, writeTestsToTestMainStep, BUILD_SEQUENCE_STEP_TYPE);


    BuildSequenceStep* createTestMainExecutableFromProjectDllAndGregTestDllStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
    createTestMainExecutableFromProjectDllAndGregTestDllStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
    createTestMainExecutableFromProjectDllAndGregTestDllStep->option->optionText = (char*)malloc(sizeof(char));
    createTestMainExecutableFromProjectDllAndGregTestDllStep->option->description = (char*)malloc(sizeof(char));
    createTestMainExecutableFromProjectDllAndGregTestDllStep->option->flagValue = (bool*)malloc(sizeof(bool)); 
    createTestMainExecutableFromProjectDllAndGregTestDllStep->function_ptr = createTestMainExecutableFromProjectDllAndGregTestDll;
    strcpy(createTestMainExecutableFromProjectDllAndGregTestDllStep->option->optionText, NO_TEST_OPTION_TEXT);
    strcpy(createTestMainExecutableFromProjectDllAndGregTestDllStep->option->description, NO_TEST_DESCRIPTION);
    createTestMainExecutableFromProjectDllAndGregTestDllStep->option->flagValue = (bool*)NO_TEST_FLAG_VALUE;
    append_ll(sequence, createTestMainExecutableFromProjectDllAndGregTestDllStep, BUILD_SEQUENCE_STEP_TYPE);

 
    BuildSequenceStep* runTestsWithExitStatusCheckStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
    runTestsWithExitStatusCheckStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
    runTestsWithExitStatusCheckStep->option->optionText = (char*)malloc(sizeof(char));
    runTestsWithExitStatusCheckStep->option->description = (char*)malloc(sizeof(char));
    runTestsWithExitStatusCheckStep->option->flagValue = (bool*)malloc(sizeof(bool)); 
    runTestsWithExitStatusCheckStep->function_ptr = runTestsWithExitStatusCheck;
    strcpy(runTestsWithExitStatusCheckStep->option->optionText, NO_TEST_OPTION_TEXT);
    strcpy(runTestsWithExitStatusCheckStep->option->description, NO_TEST_DESCRIPTION);
    runTestsWithExitStatusCheckStep->option->flagValue = (bool*)NO_TEST_FLAG_VALUE;
    append_ll(sequence, runTestsWithExitStatusCheckStep, BUILD_SEQUENCE_STEP_TYPE);


    BuildSequenceStep* compileObjectFilesIntoProjectExecutableStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
    compileObjectFilesIntoProjectExecutableStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
    compileObjectFilesIntoProjectExecutableStep->option->optionText = (char*)malloc(sizeof(char));
    compileObjectFilesIntoProjectExecutableStep->option->description = (char*)malloc(sizeof(char));
    compileObjectFilesIntoProjectExecutableStep->option->flagValue = (bool*)malloc(sizeof(bool)); 
    compileObjectFilesIntoProjectExecutableStep->function_ptr = compileObjectFilesIntoProjectExecutable;
    strcpy(compileObjectFilesIntoProjectExecutableStep->option->optionText, NULL_COMMAND_LINE_OPTION_TEXT);
    strcpy(compileObjectFilesIntoProjectExecutableStep->option->description, NULL_COMMAND_LINE_DESCRIPTION);
    compileObjectFilesIntoProjectExecutableStep->option->flagValue = (bool*)NULL_COMMAND_LINE_FLAG_VALUE;
    append_ll(sequence, compileObjectFilesIntoProjectExecutableStep, BUILD_SEQUENCE_STEP_TYPE);

    BuildSequenceStep* removeTempDirStep = (BuildSequenceStep*)malloc(sizeof(BuildSequenceStep));
    removeTempDirStep->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
    removeTempDirStep->option->optionText = (char*)malloc(sizeof(char));
    removeTempDirStep->option->description = (char*)malloc(sizeof(char));
    removeTempDirStep->option->flagValue = (bool*)malloc(sizeof(bool)); 
    removeTempDirStep->function_ptr = removeTempDir;
    strcpy(removeTempDirStep->option->optionText, DELETE_TEMP_DIR_OPTION_TEXT);
    strcpy(removeTempDirStep->option->description, DELETE_TEMP_DIR_DESCRIPTION);
    removeTempDirStep->option->flagValue = (bool*)DELETE_TEMP_DIR_FLAG_VALUE;
    append_ll(sequence, removeTempDirStep, BUILD_SEQUENCE_STEP_TYPE);
}

void freeBuildSequence(LinkedList* sequence)
{
    freeLinkedList(sequence, &freeBuildSequenceStep);
}

void freeBuildSequenceStep(void* data)
{
    BuildSequenceStep* step = (BuildSequenceStep*)data;
    free(step->function_ptr);
    free(step->option->description);
    free(step->option->flagValue);
    free(step->option->optionText);
    free(step->option);
    free(step);
}