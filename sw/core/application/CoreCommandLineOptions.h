#ifndef CORE_COMMAND_LINE_OPTIONS_H
#define CORE_COMMAND_LINE_OPTIONS_H

#include "../../external/GregCToolkit/sw/Collections/LinkedList.h"

void initAndProcessCommandLineOptions(LinkedList* options, int argc, char* argv[]);
void setCoreCommandLineOptions(LinkedList* list);
void coreCommandLineAcknowldegmentPrintouts(const LinkedList* list);

#endif