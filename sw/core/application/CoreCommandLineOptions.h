#ifndef CORE_COMMAND_LINE_OPTIONS_H
#define CORE_COMMAND_LINE_OPTIONS_H

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptionsStruct.h"
#include "../../external/GregCToolkit/sw/Collections/LinkedList/LinkedList.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif  

void initCoreCommandLineOptions(LinkedList *options);
void processCommandLineOptions(LinkedList *options, int argc, const char *argv[]);
void setCoreCommandLineOptions(LinkedList *list);
void allocateAndSetCommandLineOption(CommandLineOption* option, const char* description, const char* optionText, bool flagValue);
void coreCommandLineAcknowldegmentPrintouts(const LinkedList *list);

#ifdef __cplusplus
}
#endif  

#endif