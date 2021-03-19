#ifndef CONFIGURABLE_VALUES_H
#define CONFIGURABLE_VALUES_H

#include <stdlib.h>

#include "../../../external/GregCToolkit/sw/Collections/LinkedList/LinkedList.h"
#include "../FileStructureDefs.h"
#include "GlobalVariables.h"

#define COMPILER_CONFIG_DELIMITER ':'

static char hostCompiler_[WINDOWS_MAX_PATH_LENGTH] = "/usr/bin/gcc";
static char targetCompiler_[WINDOWS_MAX_PATH_LENGTH] = "/usr/bin/gcc";
static char projectExecutableName_[WINDOWS_MAX_PATH_LENGTH] = "BuiltByGregBuild.exe";
static char buildStartingDirectory_[WINDOWS_MAX_PATH_LENGTH] = ".";

static LinkedList* hostCompilerOptions_ = NULL;
static LinkedList* targetCompilerOptions_ = NULL;

static LinkedList* hostLinkerOptions_ = NULL;
static LinkedList* targetLinkerOptions_ = NULL;

static LinkedList* hostExcludedFiles_ = NULL;
static LinkedList* targetExcludedFiles_ = NULL;

#define CONFIG_FILE "./config"

#endif