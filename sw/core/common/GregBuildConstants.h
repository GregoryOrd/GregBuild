#ifndef GREG_TEST_CONSTANTS_H
#define GREG_TEST_CONSTANTS_H

#ifdef __WINDOWS__
#define TEST_PROJECT_LIBRARY      "TestProject.dll"
#define TEMP_TEST_PROJECT_LIBRARY "temp/TestProject.dll"
#else
#define TEST_PROJECT_LIBRARY      "libtestproject.so"
#define TEMP_TEST_PROJECT_LIBRARY "temp/libtestproject.so"
#define TEMP_TEST_PROJECT_LIBRARYLINKER_ARG "-ltestproject"
#endif

#define CURRENT_DIR "."

#define TEMP_DIR         "temp"
#define TEMP_DIR_HOST    "temp/host"
#define TEMP_DIR_TARGET  "temp/target"
#define TEMP_TEST_MAIN   "temp/TestMain"
#define TEMP_TEST_MAIN_C "temp/TestMain.c"
#define TEMP_TEST_MAIN_H "temp/TestMain.h"

#ifdef __WINDOWS__
#define TEMP_TEST_MAIN_EXE "temp/TestMain.exe"
#else
#define TEMP_TEST_MAIN_EXE "temp/TestMain"
#endif

#define DIST "dist"

#define NUM_SUPPORTED_COMMAND_LINE_OPTIONS 2

#define NO_TEST_OPTION_TEXT "--no-test-build"
#define NO_TEST_DESCRIPTION "Don't Run Any Tests"
#define NO_TEST_FLAG_VALUE  1  // The value 1 here means run the tests

#define DELETE_TEMP_DIR_OPTION_TEXT "--clean"
#define DELETE_TEMP_DIR_DESCRIPTION "Delete the temp Directory"
#define DELETE_TEMP_DIR_FLAG_VALUE  0  // The value 1 here means delete the temp directory

#define BUILD_SEQUENCE_STEP_TYPE    1
#define PLUGIN_MODULE_LL_TYPE       2
#define COMPILER_OPTION_TYPE        3
#define LINKER_OPTION_TYPE          4
#define HOST_EXCLUDED_FILE_TYPE     5
#define TARGET_EXCLUDED_FILE_TYPE   6
#define HARDWARE_SIMULATION_LIBRARY 7

#define PLUGINS_LIB_DIRECTORY          "./lib/plugins"
#define PLUGINS_LOAD_ORDER_CONFIG_FILE PLUGINS_LIB_DIRECTORY "/order"

#define DELIMITER "/"

#endif
