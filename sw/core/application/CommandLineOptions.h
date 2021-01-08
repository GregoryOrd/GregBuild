#ifndef COMMAND_LINE_OPTIONS_H
#define COMMAND_LINE_OPTIONS_H

#include <stdbool.h>

struct CommandLineOption
{
    char* optionText;
    char* description;
    bool* flagValue; //This needs to be true (1) to run the function in a BuildSequenceStep
} typedef CommandLineOption;

typedef struct CommandLineOptionList
{
    int size;
    CommandLineOption* options;
} CommandLineOptionList;

#define NO_TEST_OPTION_TEXT "--no-test-build"
#define NO_TEST_DESCRIPTION "Builds the Project Without Running Any Tests"
#define NO_TEST_FLAG_VALUE 1 //The value 1 here means run the tests

#define NULL_COMMAND_LINE_OPTION_TEXT ""
#define NULL_COMMAND_LINE_DESCRIPTION ""
#define NULL_COMMAND_LINE_FLAG_VALUE 1

void initCommandLineOptions(CommandLineOptionList* options);
void freeCommandLineOptions(CommandLineOptionList* options);
void processCommandLineArgs(int arc, char* argv[], CommandLineOptionList* options);
bool checkForOption(const CommandLineOptionList* optionsList, char* optionToFind);
void printSupportedOptions(const CommandLineOptionList* supportedOptions);
bool flagValueForOption(const CommandLineOptionList* optionsList, char* optionToFind);

#endif