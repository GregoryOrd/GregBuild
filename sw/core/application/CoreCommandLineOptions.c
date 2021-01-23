#include "CoreCommandLineOptions.h"

#include "../common/GregBuildConstants.h"

#include <stdio.h>
#include <string.h>

void initAndProcessCommandLineOptions(CommandLineOptionList* options, int argc, char* argv[])
{
    initCommandLineOptions(options, NUM_SUPPORTED_COMMAND_LINE_OPTIONS);
    setCoreCommandLineOptions(options);
    processCommandLineArgs(argc, argv, options);
    coreCommandLineAcknowldegmentPrintouts(options);
}

void setCoreCommandLineOptions(CommandLineOptionList* list)
{
    strcpy(list->options[0].optionText, NO_TEST_OPTION_TEXT);
    strcpy(list->options[0].description, NO_TEST_DESCRIPTION);
    *list->options[0].flagValue = NO_TEST_FLAG_VALUE;

    strcpy(list->options[1].optionText, DELETE_TEMP_DIR_OPTION_TEXT);
    strcpy(list->options[1].description, DELETE_TEMP_DIR_DESCRIPTION);
    *list->options[1].flagValue = DELETE_TEMP_DIR_FLAG_VALUE;
}

void coreCommandLineAcknowldegmentPrintouts(CommandLineOptionList* list)
{
    bool flag = flagValueForOption(list, NO_TEST_OPTION_TEXT);
    if(!flag)
    {
        printf("No Test Build\n");
    }
}