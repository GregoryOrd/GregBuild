#include "BuildSequence.h"

const int NUM_CORE_SEQUENCE_STEPS = 7;

void initBuildSequence(BuildSequence* sequence)
{
    sequence->size = NUM_CORE_SEQUENCE_STEPS;
    sequence->steps = (BuildSequenceStep*)malloc(NUM_CORE_SEQUENCE_STEPS * sizeof(BuildSequenceStep));
    for(int i = 0; i < NUM_CORE_SEQUENCE_STEPS; i++)
    {
        sequence->steps[i].option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
        sequence->steps[i].option->optionText = (char*)malloc(sizeof(char));
        sequence->steps[i].option->description = (char*)malloc(sizeof(char));
        sequence->steps[i].option->flagValue = (bool*)malloc(sizeof(bool));
    }

    sequence->steps[0].function_ptr = loadTestsAndSourceFiles;
    strcpy(sequence->steps[0].option->optionText, NULL_COMMAND_LINE_OPTION_TEXT);
    strcpy(sequence->steps[0].option->description, NULL_COMMAND_LINE_DESCRIPTION);
    *sequence->steps[0].option->flagValue = NULL_COMMAND_LINE_FLAG_VALUE;

    sequence->steps[1].function_ptr = compileIntoTempObjectFiles;
    strcpy(sequence->steps[1].option->optionText, NULL_COMMAND_LINE_OPTION_TEXT);
    strcpy(sequence->steps[1].option->description, NULL_COMMAND_LINE_DESCRIPTION);
    *sequence->steps[1].option->flagValue = NULL_COMMAND_LINE_FLAG_VALUE;

    sequence->steps[2].function_ptr = linkObjectFilesWithGregTestDllToMakeProjectTestDll;
    strcpy(sequence->steps[2].option->optionText, NULL_COMMAND_LINE_OPTION_TEXT);
    strcpy(sequence->steps[2].option->description, NULL_COMMAND_LINE_DESCRIPTION);
    *sequence->steps[2].option->flagValue = NULL_COMMAND_LINE_FLAG_VALUE;

    sequence->steps[3].function_ptr = writeTestsToTestMain;
    strcpy(sequence->steps[3].option->optionText, NO_TEST_OPTION_TEXT);
    strcpy(sequence->steps[3].option->description, NO_TEST_DESCRIPTION);
    *sequence->steps[3].option->flagValue = NO_TEST_FLAG_VALUE;

    sequence->steps[4].function_ptr = createTestMainExecutableFromProjectDllAndGregTestDll;
    strcpy(sequence->steps[4].option->optionText, NO_TEST_OPTION_TEXT);
    strcpy(sequence->steps[4].option->description, NO_TEST_DESCRIPTION);
    *sequence->steps[4].option->flagValue = NO_TEST_FLAG_VALUE;

    sequence->steps[5].function_ptr = runTestsWithExitStatusCheck;
    strcpy(sequence->steps[5].option->optionText, NO_TEST_OPTION_TEXT);
    strcpy(sequence->steps[5].option->description, NO_TEST_DESCRIPTION);
    *sequence->steps[5].option->flagValue = NO_TEST_FLAG_VALUE;

    sequence->steps[6].function_ptr = compileObjectFilesIntoProjectExecutable;
    strcpy(sequence->steps[6].option->optionText, NULL_COMMAND_LINE_OPTION_TEXT);
    strcpy(sequence->steps[6].option->description, NULL_COMMAND_LINE_DESCRIPTION);
    *sequence->steps[6].option->flagValue = NULL_COMMAND_LINE_FLAG_VALUE;
}

void freeBuildSequence(BuildSequence* sequence)
{
    for(int i = 0; i < sequence->size; i++)
    {
        free(sequence->steps[i].option);
        free(sequence->steps[i].option->optionText);
        free(sequence->steps[i].option->description);
        free(sequence->steps[i].option->flagValue);
    }
    free(sequence->steps);
}