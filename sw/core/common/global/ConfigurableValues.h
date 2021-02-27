#ifndef CONFIGURABLE_VALUES_H
#define CONFIGURABLE_VALUES_H

#include <stdlib.h>

#include "../../../external/GregCToolkit/sw/Collections/LinkedList/LinkedList.h"
#include "../FileStructureDefs.h"
#include "GlobalVariables.h"

#define COMPILER_CONFIG_DELIMITER ':'

char hostCompiler_[WINDOWS_MAX_PATH_LENGTH] = "/usr/bin/gcc.exe";
char targetCompiler_[WINDOWS_MAX_PATH_LENGTH] = "/usr/bin/gcc.exe";

LinkedList* hostCompilerOptions_ = NULL;
LinkedList* targetCompilerOptions_ = NULL;

LinkedList* hostLinkerOptions_ = NULL;
LinkedList* targetLinkerOptions_ = NULL;

#define COMPILER_CONFIG_FILE "./compiler.config"

#endif