#ifndef COMPILER_CONFIGURATION_VALUES
#define COMPILER_CONFIGURATION_VALUES

#include "../common/FileStructureDefs.h"

char hostCompiler_[WINDOWS_MAX_PATH_LENGTH] = "/usr/bin/gcc.exe";
char targetCompiler_[WINDOWS_MAX_PATH_LENGTH] = "/usr/bin/gcc.exe";

#define COMPILER_CONFIG_FILE "./compiler.config"

#endif