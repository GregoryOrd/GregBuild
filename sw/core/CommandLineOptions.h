#ifndef COMMAND_LINE_OPTIONS_H
#define COMMAND_LINE_OPTIONS_H

#include <stdbool.h>

#define NO_TEST_OPTION_TEXT "--no-test-build"
#define NO_TEST_DESCRIPTION "Builds the Project Without Running Any Tests"

struct CommandLineOptions
{
    bool runTests;
} typedef CommandLineOptions;

void initCommandLineOptions(CommandLineOptions* options);
void processCommandLineArgs(int arc, char* argv[], CommandLineOptions* options);

#endif