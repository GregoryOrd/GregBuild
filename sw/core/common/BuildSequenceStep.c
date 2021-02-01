#include "BuildSequenceStep.h"

#include <stdlib.h>
#include <string.h>

void allocateAndCopyBuildSequenceStep(BuildSequenceStep* dest, BuildSequenceStep* src)
{
   dest->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
   dest->option->optionText = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   dest->option->description = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   dest->option->flagValue = (bool*)malloc(sizeof(bool));
   dest->functionName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));

   strcpy(dest->option->description, src->option->description);
   strcpy(dest->option->optionText, src->option->optionText);
   dest->option->flagValue = src->option->flagValue;
   dest->function_ptr = src->function_ptr;
   strcpy(dest->functionName, src->functionName);
}

void allocateAndSetBuildSequenceStep(
    BuildSequenceStep* dest, const char* description, const char* optionText, const bool* flagValue, const STEP_FUNCTION function_ptr, const char* functionName)
{
   dest->option = (CommandLineOption*)malloc(sizeof(CommandLineOption));
   dest->option->optionText = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   dest->option->description = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));
   dest->option->flagValue = (bool*)malloc(sizeof(bool));
   dest->functionName = (char*)malloc(WINDOWS_MAX_PATH_LENGTH * sizeof(char));

   strcpy(dest->option->description, description);
   strcpy(dest->option->optionText, optionText);
   dest->option->flagValue = flagValue;
   dest->function_ptr = function_ptr;
   strcpy(dest->functionName, functionName);
}