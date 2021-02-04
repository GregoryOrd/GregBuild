#include "CoreCommandLineOptions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions_ll.h"
#include "../common/FileStructureDefs.h"
#include "../common/GregBuildConstants.h"

void initCoreCommandLineOptions(LinkedList* options)
{
   initEmptyLinkedList(options, COMMAND_LINE_OPTION_TYPE);
   setCoreCommandLineOptions(options);
}

void processCommandLineOptions(LinkedList* options, int argc, const char* argv[])
{
   processCommandLineArgs_ll(argc, argv, options, COMMAND_LINE_OPTION_TYPE);
   coreCommandLineAcknowldegmentPrintouts(options);
}

void setCoreCommandLineOptions(LinkedList* list)
{
   CommandLineOption* noTestOption = (CommandLineOption*)malloc(sizeof(CommandLineOption));
   allocateAndSetCommandLineOption(noTestOption, NO_TEST_DESCRIPTION, NO_TEST_OPTION_TEXT, NO_TEST_FLAG_VALUE);
   append_ll(list, noTestOption, COMMAND_LINE_OPTION_TYPE);

   CommandLineOption* deleteTempOption = (CommandLineOption*)malloc(sizeof(CommandLineOption));
   allocateAndSetCommandLineOption(deleteTempOption, DELETE_TEMP_DIR_DESCRIPTION, DELETE_TEMP_DIR_OPTION_TEXT, DELETE_TEMP_DIR_FLAG_VALUE);
   append_ll(list, deleteTempOption, COMMAND_LINE_OPTION_TYPE);
}

void allocateAndSetCommandLineOption(CommandLineOption* option, const char* description, const char* optionText, bool flagValue)
{
   option->optionText = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   option->description = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));

   strcpy(option->optionText, optionText);
   strcpy(option->description, description);
   option->flagValue = flagValue;
}

void coreCommandLineAcknowldegmentPrintouts(const LinkedList* list)
{
   bool flag = flagValueForOption_ll(list, NO_TEST_OPTION_TEXT, COMMAND_LINE_OPTION_TYPE);
   if (!flag)
   {
      printf("No Test Build\n");
   }
}