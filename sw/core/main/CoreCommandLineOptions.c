#include "CoreCommandLineOptions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions.h"
#include "../common/FileStructureDefs.h"
#include "../common/GregBuildConstants.h"
#include "../common/global/GlobalVariables.h"

void initCoreCommandLineOptions(CommandLineOptionList* list)
{
   list->size = 2;
   list->options = calloc(sizeof(CommandLineOption), list->size);

   strcpy(list->options[0].description, NO_TEST_DESCRIPTION);
   strcpy(list->options[0].optionText, NO_TEST_OPTION_TEXT);
   list->options[0].flagValue = NO_TEST_FLAG_VALUE;

   strcpy(list->options[1].description, DELETE_TEMP_DIR_DESCRIPTION);
   strcpy(list->options[1].optionText, DELETE_TEMP_DIR_OPTION_TEXT);
   list->options[1].flagValue = DELETE_TEMP_DIR_FLAG_VALUE;
}

void processCommandLineOptions(CommandLineOptionList* options, int argc, const char* argv[])
{
   processCommandLineArgs(argc, argv, options);
   coreCommandLineAcknowldegmentPrintouts(options);
}

void coreCommandLineAcknowldegmentPrintouts(const CommandLineOptionList* list)
{
   bool flag = flagValueForOption(list, NO_TEST_OPTION_TEXT);
   if (!flag)
   {
      printf("No Test Build\n");
   }
   setTestBuild(flag);
}