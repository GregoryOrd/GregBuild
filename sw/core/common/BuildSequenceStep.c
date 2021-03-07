#include "BuildSequenceStep.h"

#include <stdlib.h>
#include <string.h>

void copyBuildSequenceStep(BuildSequenceStep* dest, const BuildSequenceStep* src)
{
   setBuildSequenceStep(dest, src->option.description, src->option.optionText, src->option.flagValue, src->function_ptr, src->functionName);
}

void setBuildSequenceStep(BuildSequenceStep* dest, const char* description, const char* optionText, bool flagValue, const STEP_FUNCTION function_ptr, const char* functionName)
{
   strcpy(dest->option.description, description);
   strcpy(dest->option.optionText, optionText);
   dest->option.flagValue = flagValue;
   dest->function_ptr = function_ptr;
   strcpy(dest->functionName, functionName);
}