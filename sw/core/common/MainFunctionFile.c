#include "MainFunctionFile.h"

#include <string.h>

#include "MainFunctionFile_Value.h"

const char* mainFunctionFile() { return mainFunctionFilePath_; }

void setMainFunctionFile(const char* filepath) { strcpy(mainFunctionFilePath_, filepath); }