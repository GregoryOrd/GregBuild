#ifndef CORE_COMMAND_LINE_OPTIONS_H
#define CORE_COMMAND_LINE_OPTIONS_H

#include "../../external/GregCToolkit/sw/CommandLineOptions/CommandLineOptions.h"

void initAndProcessCommandLineOptions(CommandLineOptionList* options, int argc, char* argv[]);
void setCoreCommandLineOptions(CommandLineOptionList* list);
void coreCommandLineAcknowldegmentPrintouts(CommandLineOptionList* list);

#endif