#ifndef COMMAND_LINE_OPTIONS_H
#define COMMAND_LINE_OPTIONS_H

#include <stdbool.h>

/********************************************************************/
// IMPORTANT NOTE: The optionsList and descriptionsList elements     /
// must be ordered such that corresponding the option has the same   /
// index in its list as its corresponding description                /
/********************************************************************/


#define NO_TEST_OPTION_TEXT "--no-test-build"
#define NO_TEST_DESCRIPTION "Builds the Project Without Running Any Tests"

const char* optionsList[] = {
    NO_TEST_OPTION_TEXT
};

const char* descriptionsList[] = {
    NO_TEST_DESCRIPTION
};

struct CommandLineOptions
{
    bool runTests;
} typedef CommandLineOptions;



#endif