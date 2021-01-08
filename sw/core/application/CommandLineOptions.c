#include "CommandLineOptions.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initCommandLineOptions(CommandLineOptionList* list)
{
    list->size = 1;
    list->options = (CommandLineOption*)malloc(sizeof(CommandLineOption));
    
    list->options[0].description = (char*)malloc(sizeof(char*));
    list->options[0].optionText = (char*)malloc(sizeof(char*));
    list->options[0].flagValue = (bool*)malloc(sizeof(bool*));

    strcpy(list->options[0].description, NO_TEST_DESCRIPTION);
    strcpy(list->options[0].optionText, NO_TEST_OPTION_TEXT);
    *list->options[0].flagValue = NO_TEST_FLAG_VALUE;
    
}

void freeCommandLineOptions(CommandLineOptionList* list)
{
    for(int i = 0; i < list->size; i++)
    {
        free(&list->options[i].description);
        free(&list->options[i].optionText);
        free(&list->options[i].flagValue);
    }
    free(list);
}

void processCommandLineArgs(int argc, char* argv[], CommandLineOptionList* optionsList)
{
    for(int i = 1; i < argc; i++)
    {
        if(checkForOption(optionsList, argv[i]))
        {
            *optionsList->options[i-1].flagValue = !(*optionsList->options[i-1].flagValue);
        }
        else
        {
            printf("Unrecognized command line option provided: %s\n\n", argv[i]);
            printSupportedOptions(optionsList);
            exit(1);
        }
    }

    bool flag = flagValueForOption(optionsList, NO_TEST_OPTION_TEXT);
    if(!flag)
    {
        printf("No Test Build\n");
    }
}

bool checkForOption(const CommandLineOptionList* optionsList, char* optionToFind)
{
    bool optionFound = false;
    for(int i = 0; i < optionsList->size; i++)
    {
        if(strcmp(optionToFind, optionsList->options[i].optionText) == 0)
        {
            optionFound = true;
        }
    }
    return optionFound;
}

bool flagValueForOption(const CommandLineOptionList* optionsList, char* optionToFind)
{
    for(int i = 0; i < optionsList->size; i++)
    {
        if(strcmp(optionToFind, optionsList->options[i].optionText) == 0)
        {
            return *optionsList->options[i].flagValue;
        }
    }
    return true;
}

void printSupportedOptions(const CommandLineOptionList* supportedOptions)
{
    printf("Supported Options:\n");
    for(int optionNum = 0; optionNum < supportedOptions->size; optionNum++)
    {
        printf("%s    %s\n", supportedOptions->options[optionNum].optionText, supportedOptions->options[optionNum].description);
    }
}