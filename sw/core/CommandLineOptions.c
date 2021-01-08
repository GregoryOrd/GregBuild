#include "CommandLineOptions.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************************************************/
// IMPORTANT NOTE: The optionsList and descriptionsList elements     /
// must be ordered such that corresponding the option has the same   /
// index in its list as its corresponding description                /
/********************************************************************/
const char* optionsList[] = {
    NO_TEST_OPTION_TEXT
};

const char* descriptionsList[] = {
    NO_TEST_DESCRIPTION
};

void initCommandLineOptions(CommandLineOptions* options)
{
    options->runTests = true;
}

void processCommandLineArgs(int arc, char* argv[], CommandLineOptions* options)
{
    for(int i = 1; i < arc; i++)
    {
        bool optionFound = false;
        if(strcmp(argv[i], NO_TEST_OPTION_TEXT) == 0)
        {
            options->runTests = false;
            optionFound = true;
        }
        if(!optionFound)
        {
            printf("Unrecognized command line option provided: %s\n\n", argv[1]);
            printf("Supported Options:\n");
            for(int optionNum = 0; optionNum < sizeof(optionsList) / sizeof(char*); optionNum++)
            {
                printf("%s    %s\n", optionsList[optionNum], descriptionsList[optionNum]);
            }
            exit(1);
        }
    }

    if(!options->runTests)
    {
        printf("No Test Build");
    }
}