#ifndef COMPILER_CONFIGURATION_VALUES
#define COMPILER_CONFIGURATION_VALUES

#include "../common/FileStructureDefs.h"
#include "../../external/GregCToolkit/sw/Collections/LinkedList.h"

char hostCompiler_[WINDOWS_MAX_PATH_LENGTH] = "/usr/bin/gcc.exe";
char targetCompiler_[WINDOWS_MAX_PATH_LENGTH] = "/usr/bin/gcc.exe";

LinkedList* compilerOptions_ = NULL;
LinkedList* linkerOptions_ = NULL;

#define COMPILER_CONFIG_FILE "./compiler.config"

#endif