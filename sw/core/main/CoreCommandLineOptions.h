#ifndef CORE_COMMAND_LINE_OPTIONS_H
#define CORE_COMMAND_LINE_OPTIONS_H

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions.h"
#include "../../external/GregCToolkit/sw/Collections/LinkedList/LinkedList.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif  

void initCoreCommandLineOptions(CommandLineOptionList* options);
void processCommandLineOptions(CommandLineOptionList *options, int argc, const char *argv[]);
void setCoreCommandLineOptions(CommandLineOptionList* list);
void allocateAndSetCommandLineOption(CommandLineOption* option, const char* description, const char* optionText, bool flagValue);
void coreCommandLineAcknowldegmentPrintouts(const CommandLineOptionList *list);

#ifdef __cplusplus
}
#endif  

#endif