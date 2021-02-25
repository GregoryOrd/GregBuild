#ifndef COMPILER_CONFIGURATION_VALUES
#define COMPILER_CONFIGURATION_VALUES

#include "../common/FileStructureDefs.h"
#include "../../external/GregCToolkit/sw/Collections/LinkedList/LinkedList.h"

char hostCompiler_[WINDOWS_MAX_PATH_LENGTH] = "/usr/bin/gcc.exe";
char targetCompiler_[WINDOWS_MAX_PATH_LENGTH] = "/usr/bin/gcc.exe";

LinkedList* hostCompilerOptions_ = NULL;
LinkedList* targetCompilerOptions_ = NULL;

LinkedList* hostLinkerOptions_ = NULL;
LinkedList* targetLinkerOptions_ = NULL;

#define COMPILER_CONFIG_FILE "./compiler.config"

#endif